import argparse
import cProfile
import yaml

from rtc.camera import Camera
from rtc.cube import Cube
from rtc.color import Color
from rtc.lights import PointLight
from rtc.materials import Material
from rtc.matrix import *
from rtc.pattern import RingPattern, StripePattern, CheckersPattern, GradientPattern
from rtc.plane import Plane
from rtc.sphere import Sphere
from rtc.tuples import Vector, Point
from rtc.transform import *
from rtc.world import World


def parse_transform(transforms, lookup):
    transform = IdentityMatrix()
    for transform_row in transforms:
        name = transform_row[0]
        if name == "rotate-x":
            transform = matrix_mul(rotation_x(float(transform_row[1])), transform)
        elif name == "rotate-y":
            transform = matrix_mul(rotation_y(float(transform_row[1])), transform)
        elif name == "rotate-z":
            transform = matrix_mul(rotation_z(float(transform_row[1])), transform)
        elif name == "translate":
            transform = matrix_mul(translation(
                float(transform_row[1]),
                float(transform_row[2]),
                float(transform_row[3]),
            ), transform)
        elif name == "scale":
            transform = matrix_mul(scaling(
                float(transform_row[1]),
                float(transform_row[2]),
                float(transform_row[3]),
            ), transform)
        else:
            transform = parse_transform(lookup[transform_row], lookup)
    return transform


def parse_color(color_row, lookup):
    return Color(
        float(color_row[0]),
        float(color_row[1]),
        float(color_row[2]),
    )


def parse_pattern(pattern_row, lookup):
    pattern_type = pattern_row["type"]
    pattern_class = None
    if pattern_type == "checkers":
        pattern_class = CheckersPattern
    elif pattern_type == "stripes":
        pattern_class = StripePattern
    elif pattern_type == "gradient":
        pattern_class = GradientPattern
    elif pattern_type == "ring":
        pattern_class = RingPattern

    if pattern_class is None:
        raise Exception("Unknown pattern type: " + pattern_type)

    colors = pattern_row["colors"]
    color_a = parse_color(colors[0], lookup)
    color_b = parse_color(colors[1], lookup)

    pattern = pattern_class(color_a, color_b)
    if "transform" in pattern_row:
        pattern.transform = parse_transform(pattern_row["transform"], lookup)

    return pattern


def parse_material(material_row, lookup):
    if isinstance(material_row, str):
        return parse_material(lookup[material_row], lookup)
    material = Material()
    if "color" in material_row:
        material.color = parse_color(material_row["color"], lookup)
    if "ambient" in material_row:
        material.ambient = float(material_row["ambient"])
    if "diffuse" in material_row:
        material.diffuse = float(material_row["diffuse"])
    if "specular" in material_row:
        material.specular = float(material_row["specular"])
    if "shininess" in material_row:
        material.shininess = float(material_row["shininess"])
    if "reflective" in material_row:
        material.reflective = float(material_row["reflective"])
    if "pattern" in material_row:
        material.pattern = parse_pattern(material_row["pattern"], lookup)
    if "transparency" in material_row:
        material.transparency = float(material_row["transparency"])
    if "refractive-index" in material_row:
        material.refractive_index = float(material_row["refractive-index"])
    return material


def parse_shape(row, lookup):
    shape = row["add"]
    shape_class = None
    if shape == "plane":
        shape_class = Plane
    elif shape == "sphere":
        shape_class = Sphere
    elif shape == "cube":
        shape_class = Cube

    if shape_class is None:
        raise Exception("Unknown shape: " + shape)

    transform = parse_transform(row["transform"], lookup)
    material = parse_material(row["material"], lookup)

    s = shape_class(material)
    s.transform = transform

    if "shadow" in row:
        s.cast_shadow = row["shadow"]

    return s


def parse_point(row, lookup):
    return Point(
        float(row[0]),
        float(row[1]),
        float(row[2]),
    )


def parse_vector(row, lookup):
    return Vector(
        float(row[0]),
        float(row[1]),
        float(row[2]),
    )


def parse_camera(row, lookup):
    width = int(row["width"])
    height = int(row["height"])
    fov = float(row["field-of-view"])
    at = parse_point(row["from"], lookup)
    to = parse_point(row["to"], lookup)
    up = parse_vector(row["up"], lookup)

    camera = Camera(width, height, fov)
    camera.transform = ViewTransform(at, to, up)
    return camera


def parse_light(row, lookup):
    at = parse_point(row["at"], lookup)
    intensity = parse_color(row["intensity"], lookup)
    return PointLight(at, intensity)


def render_file(file, output_file):
    with open(file, "r") as fp:
        parsed_yaml = yaml.safe_load(fp)
        defines = filter(lambda row: "define" in row, parsed_yaml)
        adds = filter(lambda row: "add" in row, parsed_yaml)

        lookup = {}
        for row in defines:
            key = row["define"]
            value = row["value"]
            lookup[key] = value
            if "extend" in row:
                extension = row["extend"]
                lookup[key] = {
                    **lookup[extension],
                    **lookup[key],
                }

        shapes = []
        lights = []
        camera = None
        for row in adds:
            add = row["add"]
            if add == "camera":
                camera = parse_camera(row, lookup)
            elif add == "light":
                lights.append(parse_light(row, lookup))
            else:
                shapes.append(parse_shape(row, lookup))

        if len(lights) == 0:
            raise Exception("No lights provided by configuration")

        for light in lights:
            if light is None:
                raise Exception("Invalid lights provided by configuration")

        if camera is None:
            raise Exception("No camera provided by configuration")

        world = World(shapes, lights)
        image = camera.render(world, progress=True)
        image.save(output_file)


def main():
    parser = argparse.ArgumentParser(description="Ray Tracer Challenge")
    parser.prog = "rtc"

    parser.add_argument(help="yaml configuration file", dest="file")
    parser.add_argument(help="where to save the generated image", dest="output_file")

    args = parser.parse_args()

    if args.file is None or args.output_file is None:
        parser.print_help()
        exit(1)

    render_file(args.file, args.output_file)


if __name__ == "__main__":
    cProfile.run("main()", filename="rtc.prof", sort="time")

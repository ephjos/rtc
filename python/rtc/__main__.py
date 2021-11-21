import argparse
import yaml

from rtc.camera import Camera
from rtc.color import Color
from rtc.lights import PointLight
from rtc.materials import Material
from rtc.pattern import RingPattern, StripePattern, CheckersPattern, GradientPattern
from rtc.plane import Plane
from rtc.sphere import Sphere
from rtc.tuples import Vector, Point
from rtc.transform import Transform, ViewTransform
from rtc.world import World

SHAPES = ['plane', 'sphere']

def parse_transform(transforms, lookup):
    transform = Transform()
    for transform_row in transforms:
        name = transform_row[0]
        if name == 'rotate-x':
            transform = transform.rotation_x(float(transform_row[1]))
        elif name == 'rotate-y':
            transform = transform.rotation_y(float(transform_row[1]))
        elif name == 'rotate-z':
            transform = transform.rotation_z(float(transform_row[1]))
        elif name == 'translate':
            transform = transform.translation(
                float(transform_row[1]),
                float(transform_row[2]),
                float(transform_row[3]),
            )
        elif name == 'scale':
            transform = transform.scaling(
                float(transform_row[1]),
                float(transform_row[2]),
                float(transform_row[3]),
            )
    return transform

def parse_color(color_row, lookup):
    return Color(
        float(color_row[0]),
        float(color_row[1]),
        float(color_row[2]),
    )

def parse_pattern(pattern_row, lookup):
    pattern_type = pattern_row['type']
    pattern_class = None
    if pattern_type == 'checkers':
        pattern_class = CheckersPattern
    elif pattern_type == 'stripes':
        pattern_class = StripePattern
    elif pattern_type == 'gradient':
        pattern_class = GradientPattern
    elif pattern_type == 'ring':
        pattern_class = RingPattern

    if pattern_class is None:
        raise Exception('Unknown pattern type: ' + pattern_type)

    colors = pattern_row['colors']
    color_a = parse_color(colors[0], lookup)
    color_b = parse_color(colors[1], lookup)

    pattern = pattern_class(color_a, color_b)
    if 'transform' in pattern_row:
        pattern.transform = parse_transform(pattern_row['transform'], lookup)

    return pattern

def parse_material(material_row, lookup):
    if isinstance(material_row, str):
        return parse_material(lookup[material_row], lookup)
    material = Material()
    if 'color' in material_row:
        material.color = parse_color(material_row['color'], lookup)
    if 'ambient' in material_row:
        material.ambient = float(material_row['ambient'])
    if 'diffuse' in material_row:
        material.diffuse = float(material_row['diffuse'])
    if 'specular' in material_row:
        material.specular = float(material_row['specular'])
    if 'shininess' in material_row:
        material.shininess = float(material_row['shininess'])
    if 'reflective' in material_row:
        material.reflective = float(material_row['reflective'])
    if 'pattern' in material_row:
        material.pattern = parse_pattern(material_row['pattern'], lookup)
    if 'transparency' in material_row:
        material.transparency = float(material_row['transparency'])
    if 'refractive-index' in material_row:
        material.refractive_index = float(material_row['refractive-index'])
    return material


def parse_plane(row, lookup):
    transform = parse_transform(row['transform'], lookup)
    material = parse_material(row['material'], lookup)

    p = Plane(material)
    p.transform = transform
    return p


def parse_sphere(row, lookup):
    transform = parse_transform(row['transform'], lookup)
    material = parse_material(row['material'], lookup)

    s = Sphere(material)
    s.transform = transform
    return s


def parse_shape(row, lookup):
    shape = row['add']
    if shape == 'plane':
        return parse_plane(row, lookup)
    elif shape == 'sphere':
        return parse_sphere(row, lookup)

    raise Exception('Unknown shape: ' + shape)


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
    width = int(row['width'])
    height = int(row['height'])
    fov = float(row['field-of-view'])
    at = parse_point(row['from'], lookup)
    to = parse_point(row['to'], lookup)
    up = parse_vector(row['up'], lookup)

    camera = Camera(width, height, fov)
    camera.transform = ViewTransform(at, to, up)
    return camera


def parse_light(row, lookup):
    at = parse_point(row['at'], lookup)
    intensity = parse_color(row['intensity'], lookup)
    return PointLight(at, intensity)


def render_file(file, output_file):
    with open(file, "r") as fp:
        parsed_yaml = yaml.safe_load(fp)
        defines = filter(lambda row: 'define' in row, parsed_yaml)
        adds = filter(lambda row: 'add' in row, parsed_yaml)

        lookup = {}
        for row in defines:
            key = row['define']
            value = row['value']
            lookup[key] = value

        shapes = []
        lights = []
        camera = None
        for row in adds:
            add = row['add']
            if add in SHAPES:
                shapes.append(parse_shape(row, lookup))
            elif add == 'camera':
                camera = parse_camera(row, lookup)
            elif add == 'light':
                lights.append(parse_light(row, lookup))

        if len(lights) == 0:
            raise Exception('No lights provided by configuration')

        for light in lights:
            if light is None:
                raise Exception('Invalid lights provided by configuration')

        if camera is None:
            raise Exception('No camera provided by configuration')

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
    main()

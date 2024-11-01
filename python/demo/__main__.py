import argparse

from demo.demo_canvas import demo_canvas
from demo.demo_camera import demo_camera
from demo.demo_material import demo_material
from demo.demo_pattern import demo_pattern
from demo.demo_plane import demo_plane
from demo.demo_ray import demo_ray
from demo.demo_reflection import demo_reflection
from demo.demo_refraction import demo_refraction
from demo.demo_transform import demo_transform
from demo.demo_tuples import demo_tuples


def demo_all(*args):
    demo_canvas()
    demo_camera()
    demo_material()
    demo_plane()
    demo_ray()
    demo_reflection()
    demo_refraction()
    demo_transform()
    demo_tuples()


def main():
    parser = argparse.ArgumentParser(description="Ray Tracer Challenge Demos")
    parser.prog = "demo"

    subparsers = parser.add_subparsers(help="available demos", dest="demo")

    # Add parser
    subparsers.add_parser("all", help="Runs all demos")
    subparsers.add_parser("canvas", help="demonstrate using canvas")
    subparsers.add_parser("camera", help="demonstrate using camera")
    subparsers.add_parser("material", help="demonstrate using material")
    subparsers.add_parser("pattern", help="demonstrate using pattern")
    subparsers.add_parser("plane", help="demonstrate using plane")
    subparsers.add_parser("ray", help="demonstrate using rays")
    subparsers.add_parser("reflection", help="demonstrate reflections")
    subparsers.add_parser("refraction", help="demonstrate refractions")
    subparsers.add_parser(
        "transform",
        help="demonstrate using basic transformation matrices to build a clock",
    )
    subparsers.add_parser("tuples", help="demonstrate using basic tuples")

    args = parser.parse_args()

    if args.demo is None:
        parser.print_help()
        exit(1)

    # Map parser name to function
    commands = {
        "all": demo_all,
        "canvas": demo_canvas,
        "camera": demo_camera,
        "material": demo_material,
        "pattern": demo_pattern,
        "plane": demo_plane,
        "ray": demo_ray,
        "reflection": demo_reflection,
        "refraction": demo_refraction,
        "transform": demo_transform,
        "tuples": demo_tuples,
    }

    commands[args.demo](args)


if __name__ == "__main__":
    main()

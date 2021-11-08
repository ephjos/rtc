import argparse

from demo.demo_canvas import demo_canvas
from demo.demo_camera import demo_camera
from demo.demo_material import demo_material
from demo.demo_ray import demo_ray
from demo.demo_transform import demo_transform
from demo.demo_tuples import demo_tuples


def main():
    parser = argparse.ArgumentParser(description="Ray Tracer Challenge Demos")
    parser.prog = "demo"

    subparsers = parser.add_subparsers(help="available demos", dest="demo")

    # Add parser
    subparsers.add_parser("canvas", help="demonstrate using canvas")
    subparsers.add_parser("camera", help="demonstrate using camera")
    subparsers.add_parser(
        "material", help="demonstrate using material"
    )
    subparsers.add_parser("ray", help="demonstrate using rays")
    subparsers.add_parser(
        "transform",
        help="demonstrate using basic transformation matrices to build a clock",
    )
    subparsers.add_parser(
        "tuples", help="demonstrate using basic tuples"
    )

    args = parser.parse_args()

    if args.demo is None:
        parser.print_help()
        exit(1)

    # Map parser name to function
    commands = {
        "canvas": demo_canvas,
        "camera": demo_camera,
        "material": demo_material,
        "ray": demo_ray,
        "transform": demo_transform,
        "tuples": demo_tuples,
    }

    commands[args.demo](args)


if __name__ == "__main__":
    main()

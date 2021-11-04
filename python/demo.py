#!/usr/bin/env python
import argparse

from canvas import demo_canvas
from sphere import demo_ray
from transform import demo_transform
from tuples import demo_tuples

def main():
    parser = argparse.ArgumentParser(description='Ray Tracer Challenge Demos')
    subparsers = parser.add_subparsers(help='available demos', dest="demo")

    # Add parser
    canvas_parser = subparsers.add_parser('canvas', help='demonstrate using canvas')
    ray_parser = subparsers.add_parser('ray', help='demonstrate using rays')
    transform_parser = subparsers.add_parser('transform', help='demonstrate using basic transformation matrices to build a clock')
    tuples_parser = subparsers.add_parser('tuples', help='demonstrate using basic tuples')

    args = parser.parse_args()

    if args.demo is None:
        parser.print_help()
        exit(1)

    # Map parser name to function
    commands = {
        'canvas': demo_canvas,
        'ray': demo_ray,
        'transform': demo_transform,
        'tuples': demo_tuples,
    }

    commands[args.demo](args)

if __name__ == "__main__":
    main()


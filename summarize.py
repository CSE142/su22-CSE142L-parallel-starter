#!/opt/conda/bin/python

import pandas as pd
from notebook import render_csv, pa_columns
import click

@click.command()
@click.argument("inputs", required = True, nargs=-1)
@click.option("--out", "-o", type=click.File(mode="w"), default="-")
@click.option("--func", "-f", type=click.Choice(["max", "min", "mean"]), default="mean")
def main(inputs, out, func):

    df = render_csv(inputs, columns=pa_columns)
    df = df.groupby(by=["function", "thread", "size", "power", "p1", "p2", "p3", "p4", "p5"])

    if func == "max":
        df = df.max()
    elif func == "min":
        df = df.min()
    elif func == "mean":
        df = df.min()
        
    df.to_csv(out)

if __name__ == '__main__':
    main()

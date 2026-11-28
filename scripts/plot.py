import argparse
from pathlib import Path

import matplotlib

matplotlib.use("Agg")

import matplotlib.pyplot as plt
import pandas as pd

SURFACE = "#fcfcfb"
TEXT_PRIMARY = "#0b0b0b"
TEXT_SECONDARY = "#52514e"
GRID = "#e4e3df"
SERIES = "#2a78d6"


def latest_csv(root: Path) -> Path:
    files = sorted(root.glob("**/metrics/*.csv"), key=lambda p: p.stat().st_mtime)
    if not files:
        raise SystemExit(f"no metrics files under {root}")
    return files[-1]


def style(ax, ylabel: str) -> None:
    ax.set_facecolor(SURFACE)
    ax.grid(axis="y", color=GRID, linewidth=0.8)
    ax.set_axisbelow(True)
    for side in ("top", "right"):
        ax.spines[side].set_visible(False)
    for side in ("left", "bottom"):
        ax.spines[side].set_color(GRID)
    ax.tick_params(colors=TEXT_SECONDARY, labelsize=9)
    ax.set_ylabel(ylabel, color=TEXT_SECONDARY)


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("csv", nargs="?", type=Path)
    parser.add_argument("--window", type=int, default=50)
    args = parser.parse_args()

    path = args.csv or latest_csv(Path("runs"))
    data = pd.read_csv(path)
    window = max(1, min(args.window, len(data)))

    fig, axes = plt.subplots(2, 1, sharex=True, figsize=(9, 6), facecolor=SURFACE)

    for ax, column in zip(axes, ("reward", "steps")):
        smooth = data[column].rolling(window, min_periods=1).mean()
        ax.plot(data["episode"], data[column], color=SERIES, alpha=0.3, linewidth=1,
                label="per episode")
        ax.plot(data["episode"], smooth, color=SERIES, linewidth=2,
                label=f"mean of last {window}")
        style(ax, column)

    axes[0].legend(frameon=False, labelcolor=TEXT_SECONDARY, fontsize=9, loc="lower right")
    axes[1].set_xlabel("episode", color=TEXT_SECONDARY)

    env, agent = path.parts[-4], path.parts[-3]
    fig.suptitle(f"{env} · {agent} · {path.stem}", color=TEXT_PRIMARY, x=0.08, ha="left")

    output = path.with_suffix(".png")
    fig.savefig(output, dpi=150, bbox_inches="tight")
    print(output)


if __name__ == "__main__":
    main()

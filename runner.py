import subprocess
from pathlib import Path
from rich.live import Live
from rich.console import Console
from rich.panel import Panel
from rich.progress import Progress, SpinnerColumn, TextColumn, BarColumn, TimeRemainingColumn
from rich.progress import TimeElapsedColumn
EXPORT_FOLDER = "export_data/bulk/"
SKIPPED_GRAPHS = []
FOLDERS_TO_PROCESS = ["data/4_edge_critical_snarks.10.g6",
                      "data/4_edge_critical_snarks.18.g6",
                      "data/4_edge_critical_snarks.20.g6",
                      "data/4_edge_critical_snarks.22.g6",
                      "data/4_edge_critical_snarks.26.g6",
                      "data/4_edge_critical_snarks.28.g6",
                      "data/4_edge_critical_snarks.30.g6",
                      "data/4_edge_critical_snarks.32.g6",
                      "data/4_edge_critical_snarks.34.g6",
                      ]


BINARY = "./cmake-build-debug/rocnikovy_projekt"

"""

Store which graph we are currently working on
"""

FLAG_ONE_COLOR = False
def run(format:str, file_name:str,file_index: int, max_index:int = -1, progress = None):
    if progress == None:
        raise "Progress object needed"

    file_name = file_name.removeprefix("data/").removesuffix(".g6")

    out_dir = Path(EXPORT_FOLDER) / (("OC_" if FLAG_ONE_COLOR else "") + file_name)
    out_dir.mkdir(parents=True, exist_ok=True)

    out = out_dir / f"{file_name}.{file_index}{f'_{max_index}' if max_index != -1 else ''}.csv"
    if out.exists():
        progress.console.print(f"Skipping {out} (already exists)")
        return "SKIPPED"

    cmd = [
        BINARY,
        "-g", format,
        "-f", out,
    ]

    if(FLAG_ONE_COLOR):
        cmd.append("--one_color")

    result = subprocess.run(
        cmd,
        capture_output=True,
        text=True
    )

    if result.returncode == 0:
        progress.console.print(f"{format} successfully calculated")
        return "SUCCESS"

    else:
        SKIPPED_GRAPHS.append(format)
        progress.console.print(f"{format} There was an error")
        return "ERROR"



if __name__ == "__main__":
    console = Console()



    with Progress(
            SpinnerColumn(),
            TextColumn("[bold blue]{task.description}"),
            BarColumn(),
            TextColumn("{task.completed}/{task.total}"),
            TimeElapsedColumn(),
            console=console,
    ) as progress:

        overall = progress.add_task(
            "[green]Total datasets", total=len(FOLDERS_TO_PROCESS)
        )

        for file in FOLDERS_TO_PROCESS:
            processed = 0
            skipped = 0
            errors = 0

            lines = [line.strip() for line in open(file)]
            total = len(lines)

            task = progress.add_task(f"[cyan]{file}", total=total)

            for i in range(total):
                result = run(lines[i], file, i + 1, total, progress)

                if result == "SUCCESS":
                    processed += 1
                elif result == "SKIPPED":
                    skipped += 1
                else:
                    errors += 1

                progress.update(
                    task,
                    advance=1,
                    description=(
                        f"{file} | "
                        f"[green]done:{processed}[/green] "
                        f"[yellow]skip:{skipped}[/yellow] "
                        f"[red]err:{errors}[/red]"
                    ),
                )

            progress.update(overall, advance=1)

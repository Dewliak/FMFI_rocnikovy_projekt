import subprocess
from pathlib import Path
from rich.live import Live
from rich.console import Console
from rich.panel import Panel

EXPORT_FOLDER = "export_data/bulk/"

FOLDERS_TO_PROCESS = ["data/4_edge_critical_snarks.10.g6",
                      "data/4_edge_critical_snarks.18.g6",
                      "data/4_edge_critical_snarks.20.g6",
                      "data/4_edge_critical_snarks.22.g6",
                      ]


BINARY = "./cmake-build-debug/rocnikovy_projekt"

"""

Store which graph we are currently working on
"""


def run(format:str, file_name:str,file_index: int, max_index:int = -1):

    file_name = file_name.removeprefix("data/").removesuffix(".g6")
    out = Path(EXPORT_FOLDER) / f"{file_name}.{file_index}{f'_{max_index}' if max_index != -1 else ''}.csv"
    if out.exists():
        print(f"Skipping {out} (already exists)")
        return

    cmd = [
        BINARY,
        "-g", format,
        "-f", out,
    ]

    result = subprocess.run(
        cmd,
        capture_output=True,
        text=True
    )

    if result.returncode == 0:
        print(f"{format} successfully calculated")

    else:
        print(f"{format} There was an error")



if __name__ == "__main__":

    console = Console()

for file in FOLDERS_TO_PROCESS:
    lines = []

    with open(file, "r") as f:
        for line in f:
            lines.append(line.strip())

    total = len(lines)
    current_file = file

    with Live(refresh_per_second=10) as live:
        for i in range(total):
            live.update(
                Panel.fit(
                    f"[bold]File:[/bold] {file}\n"
                    f"[bold]Progress:[/bold] {i+1}/{total}"
                )
            )

            run(lines[i], file, i + 1, total)

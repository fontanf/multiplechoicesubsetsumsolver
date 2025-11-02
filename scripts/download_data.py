import gdown
import pathlib
import py7zr

gdown.download(id="1kJQIfX-OwolnWcYZ8U0GQOmx8A0CSmiB", output="data.7z")
with py7zr.SevenZipFile("data.7z", mode="r") as z:
    z.extractall(path="data")
pathlib.Path("data.7z").unlink()

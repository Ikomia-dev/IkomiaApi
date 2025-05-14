from pathlib import PureWindowsPath


def to_posix_path(path_str: str) -> str:
    """
    Convert any input path (Windows or POSIX) to a clean POSIX-style path.

    Args:
        path_str (str): The input path string.

    Returns:
        str: A POSIX-style path (with forward slashes, and properly parsed).
    """
    return str(PureWindowsPath(path_str).as_posix())

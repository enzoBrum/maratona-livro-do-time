#!/usr/bin/python

# SOURCE: https://github.com/jaehyunp/stanfordacm/blob/master/generate_pdf.py

import os
import subprocess

code_dir = "."
title = "UFSC livro do time"


def get_sections() -> list[tuple[str, list[tuple[str, str]]]]:
    sections = []
    for root, dirs, files in sorted(os.walk(".")):
        if ".git" in dirs:
            dirs.remove(".git")
        if root == ".":
            continue

        # ignore ./
        print(files)
        cur_section = root[2:].replace("-", " ").replace("_", " ").title()
        cur_files = [
            (os.path.join(root[2:], file), file.split(".")[0].replace("_", " ").title())
            for file in sorted(files)
            if file.split(".")[-1]
            in (
                "py",
                "cpp",
                "cxx",
                "c",
                "java",
            )
        ]
        if not cur_files:
            continue
        sections.append((cur_section, cur_files))

    return sections


def get_style(filename):
    ext = filename.lower().split(".")[-1]
    if ext in ["c", "cc", "cpp"]:
        return "cpp"
    elif ext in ["java"]:
        return "java"
    elif ext in ["py"]:
        return "py"
    elif ext in ["json"]:
        return "json"
    else:
        return "txt"


# TODO: check if this is everything we need
def texify(s):
    # s = s.replace('\'', '\\\'')
    # s = s.replace('\"', '\\\"')
    return s


def get_tex(sections):
    tex = ""
    for section_name, subsections in sections:
        tex += "\\section{%s}\n" % texify(section_name)
        for filename, subsection_name in subsections:
            tex += "\\subsection{%s}\n" % texify(subsection_name)
            tex += "\\raggedbottom\\lstinputlisting[style=%s]{%s/%s}\n" % (
                get_style(filename),
                code_dir,
                filename,
            )
            tex += "\\hrulefill\n"
        tex += "\n"
    return tex


if __name__ == "__main__":
    sections = get_sections()
    print(sections)
    tex = get_tex(sections)

    with open("contents.tex", "w") as f:
        f.write(tex)

    if not os.getenv("CI_CD"):
        latexmk_options = ["latexmk", "-pdf", "notebook.tex"]
        subprocess.call(latexmk_options)

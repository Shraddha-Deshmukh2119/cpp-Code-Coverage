import argparse
import json
import os
import xml.etree.ElementTree as ET


# ==========================================
# PARSE JACOCO XML
# ==========================================

def parse_jacoco(jacoco_path):

    result = {
        "language": "java",
        "files": [],
        "summary": {}
    }

    tree = ET.parse(jacoco_path)
    root = tree.getroot()

    total_line_missed = 0
    total_line_covered = 0

    total_branch_missed = 0
    total_branch_covered = 0

    for package in root.findall("package"):

        for clazz in package.findall("class"):

            file_data = {
                "name": clazz.attrib.get("name"),
                "line_percent": 0,
                "branch_percent": 0
            }

            for counter in clazz.findall("counter"):

                ctype = counter.attrib.get("type")

                missed = int(counter.attrib.get("missed"))
                covered = int(counter.attrib.get("covered"))

                total = missed + covered

                percent = 0

                if total > 0:
                    percent = round((covered / total) * 100, 2)

                if ctype == "LINE":
                    file_data["line_percent"] = percent

                    total_line_missed += missed
                    total_line_covered += covered

                if ctype == "BRANCH":
                    file_data["branch_percent"] = percent

                    total_branch_missed += missed
                    total_branch_covered += covered

            result["files"].append(file_data)

    line_total = total_line_missed + total_line_covered
    branch_total = total_branch_missed + total_branch_covered

    result["summary"] = {

        "line_percent":
            round((total_line_covered / line_total) * 100, 2)
            if line_total else 0,

        "branch_percent":
            round((total_branch_covered / branch_total) * 100, 2)
            if branch_total else 0
    }

    return result


# ==========================================
# PARSE GCOVR XML
# ==========================================

def parse_gcovr(gcovr_path):

    result = {
        "language": "cpp",
        "files": [],
        "summary": {}
    }

    if not os.path.exists(gcovr_path):
        return result

    tree = ET.parse(gcovr_path)
    root = tree.getroot()

    line_rate = float(root.attrib.get("line-rate", 0))
    branch_rate = float(root.attrib.get("branch-rate", 0))

    result["summary"] = {

        "line_percent": round(line_rate * 100, 2),

        "branch_percent": round(branch_rate * 100, 2)
    }

    for clazz in root.findall(".//class"):

        file_data = {

            "name": clazz.attrib.get("filename"),

            "line_percent":
                round(float(clazz.attrib.get("line-rate", 0)) * 100, 2),

            "branch_percent":
                round(float(clazz.attrib.get("branch-rate", 0)) * 100, 2)
        }

        result["files"].append(file_data)

    return result


# ==========================================
# MAIN
# ==========================================

def main():

    parser = argparse.ArgumentParser()

    parser.add_argument("--jacoco_xml")
    parser.add_argument("--surefire_dir")
    parser.add_argument("--gcovr_xml")
    parser.add_argument("--gcovr_json")
    parser.add_argument("--output")

    args = parser.parse_args()

    java_data = parse_jacoco(args.jacoco_xml)

    cpp_data = parse_gcovr(args.gcovr_xml)

    unified = {

        "project": "BMC Project",

        "coverage": {

            "java": java_data,

            "cpp": cpp_data
        }
    }

    with open(args.output, "w") as f:

        json.dump(unified, f, indent=4)

    print("Unified JSON generated successfully.")


if __name__ == "__main__":
    main()
#!/usr/bin/env python3

"""
@file     python.py
@date     DD/MM/YYYY
@version  0.0.1
@license  GNU General Public License v2.0
@url      github.com/Julynx/RepositoryName
@author   Julio Cabria (username@domain.ext)
"""

import subprocess
import functools
import time
from contextlib import suppress
import sys


def grab(text, start, end):
    # Find the first occurence of the start string
    start_index = text.find(start)
    # Starting from start_index, find the first occurence of the end string
    end_index = text.find(end, start_index + len(start))
    # Return the text between the start and end strings
    return text[start_index + len(start):end_index]


def run_cmd(cmd) -> str:
    """
    Run a command and return the output
    """
    try:
        output = subprocess.check_output(cmd, shell=True)
    except subprocess.CalledProcessError:
        output = ""
    return output.decode("utf-8").strip()


def progress_bar(iteration, total, max_width=32, info=""):
    bar_width = int((iteration/(total - 1)) * max_width)
    extra = f"  {info}" if info else f"  {iteration}/{total}"
    return "█" * bar_width + "░" * (max_width - bar_width) + extra


def print_progress_bar(iteration, total, max_width=32, info="", end="\r"):
    print(progress_bar(iteration, total, max_width, info), end=end)


def timer(func):
    """Print the runtime of the decorated function"""
    @functools.wraps(func)
    def wrapper_timer(*args, **kwargs):
        start_time = time.perf_counter()
        value = func(*args, **kwargs)
        end_time = time.perf_counter()
        run_time = end_time - start_time
        print(f"Finished {func.__name__!r} in {run_time:.4f} s")
        return value
    return wrapper_timer


def args(positional=[]):
    """
    Simple argument parser.

    Example:
    $: program joe 1234 -keep -host=127.0.0.1

    dictionary = args(["username", "password"])

    >> username:    joe
    >> password:    1234
    >> -keep:       True
    >> -host:       127.0.0.1

    Args:
        positional (str): A list of strings for the positional arguments.

    Returns:
        dict: A dictionary containing the argument names and their values.
    """
    args_dict = {}

    # Store positional arguments
    tail = len(positional)
    for i, pos_arg in enumerate(positional):
        with suppress(IndexError):
            if str(sys.argv[i+1]).startswith("-"):
                tail = i
                break
            value = sys.argv[i+1]
            args_dict[pos_arg] = value

    # Store flags
    for i in range(tail+1, len(sys.argv)):
        try:
            value = str(sys.argv[i]).split("=")[1]
        except IndexError:
            value = True
        args_dict[str(sys.argv[i]).split("=")[0]] = value

    return args_dict


def test(currentValue, expectedValue):
    global passed, failed, last_error

    try:
        passed
        failed
    except NameError:
        passed = 0
        failed = 0

    last_error = ""

    if currentValue == expectedValue:
        passed += 1
        return '🟢'
    else:
        failed += 1
        last_error = f"- Expected {expectedValue}, got {currentValue}"
        return '🔴'


def reset_test_counters():
    global passed, failed
    passed, failed = 0, 0


def get_test_counters():
    global passed, failed

    try:
        passed
        failed
    except NameError:
        passed = 0
        failed = 0

    return passed, failed, passed + failed


def test_report():
    passed, _, total = get_test_counters()
    return f"{test(passed, total)} {passed}/{total} tests passed"

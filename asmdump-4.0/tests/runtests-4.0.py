#!/usr/bin/env python

# To work on both Python 2 and 3
from __future__ import print_function

"""
This script is to test the asmdump 3.0

You should run it as:

    $ python test/runtests-3.0.py ./asmdump.exe
"""

import sys
import os
import zipfile
import tempfile
import contextlib
import shutil
import subprocess


instructions = {
    0x00: "invalid",
    # Binary instructions
    0x01: "mov",
    0x04: "add",
    0x05: "sub",
    0x06: "mul",
    0x07: "div",
    0x08: "and",
    0x09: "or",
    0x0a: "xor",
    0x0b: "cmp",
    0x0c: "mod",
    # Unary instructions
    0x14: "not",
    # Jump instructions
    0x20: "jmp",
    0x21: "jle",
    0x22: "jl",
    0x23: "je",
    0x24: "jne",
    0x25: "jge",
    0x26: "jg",
    # Operandless instructions
    0x30: "syscall",
}

#
# main command line entry point. Runs all tests
#

def main(exename):
    '''Check that zip file at zippath meets the spec'''

    tests = [
        (test_running_noargs, 'Running program with no arguments'),
        (test_running_version, 'Running with --version'),
        (test_running_toomany_args, 'Running with too many arguments'),
        (test_running_mov, 'Running with 0x01'),
        (test_running_bad_opcode_str, 'Running with 0x1blah'),
        (test_running_opcodes_all, 'Running with all opcodes'),
        (test_running_invalid_opcodes, 'Running with invalid opcodes'),
        (test_running_hexdump_invalid, 'Running hexdump invalid filename'),
        (test_running_hexdump, 'Running hexdump with test files'),
        (test_running_hexdump_stdin, 'Running hexdump with test files on stdin'),
        (test_running_asmdump, 'Running asmdump with tests/exit.asm'),
    ]

    # Loop through all tests...
    for testfunc, description in tests:
        print('Checking %s ...' % description)
        if not testfunc(exename):
            return 1
        else:
            print('Okay!\n')

    print(' ---------  All tests passed! ------------ \n')
    return 0

#
# The test functions.
#

USAGE = '''Usage:
./asmdump.exe --version
./asmdump.exe --opcode OPCODE
./asmdump.exe --hexdump [FILE]
./asmdump.exe FILE
'''

def test_running_noargs(exename):
    args = []
    out = ''
    err = USAGE
    ret = 1
    return test_with_args(exename, args, out, err, ret)

def test_running_version(exename):
    args = ['--version']
    out = '4.0\n'
    err = ''
    ret = 0
    return test_with_args(exename, args, out, err, ret)

def test_running_toomany_args(exename):
    out = ''
    err = USAGE
    ret = 1
    for n in 3, 4, 5:
        args = ['foo'] * n
        if not test_with_args(exename, args, out, err, ret):
            return False

    return True

def test_running_mov(exename):
    args = ['--opcode', '0x01']
    out = 'mov\n'
    err = ''
    ret = 0
    return test_with_args(exename, args, out, err, ret)

def test_running_bad_opcode_str(exename):
    args = ['--opcode', '0x1blah']
    out = ''
    err = 'Invalid opcode string "0x1blah"\n'
    ret = 1
    return test_with_args(exename, args, out, err, ret)

def test_running_opcodes_all(exename):
    err = ''
    ret = 0
    for opcode, mnemonic in sorted(instructions.items()):
        args = ['--opcode', '0x%02x' % opcode]
        out = mnemonic + '\n'
        if not test_with_args(exename, args, out, err, ret):
            return False
    return True

def test_running_invalid_opcodes(exename):
    out = ''
    ret = 1
    for n in range(256):
        if n in instructions:
            continue
        args = ['--opcode', '0x%02x' % n]
        err = 'No such instruction "0x%02x"\n' % n
        if not test_with_args(exename, args, out, err, ret):
            return False
    return True

def test_running_hexdump_invalid(exename):
    filename = 'invalid_invalid.txt'
    args = ['--hexdump', filename]
    err = 'No such file "%s"\n' % filename
    return test_with_args(exename, args, '', err, 1)

hd_filenames = 'hello.c', '127.dat', 'helloasm.asm'

def test_running_hexdump(exename):
    for filename in hd_filenames:
        filepath = os.path.join('tests', filename)
        args = ['--hexdump', filepath]
        with open(filepath + '.txt') as expected_file:
            expected_output = expected_file.read()
        if not test_with_args(exename, args, expected_output, '', 0):
            return False
    return True

asm_filenames = [
    'exit.out',
    'hello_bad.out',
    'hello.out',
    'bigsmall.out',
    'everything.out'
]

def test_running_asmdump(exename):
    for filename in asm_filenames:
        filepath = os.path.join('tests', filename)
        expected_filename = os.path.splitext(filepath)[0] + '.asm'
        expected_errname = os.path.splitext(filepath)[0] + '.err'
        args = [filepath]
        if os.path.exists(expected_errname):
            with open(expected_errname) as expected_file:
                err = expected_file.read()
            out = ''
        else:
            with open(expected_filename) as expected_file:
                out = expected_file.read()
            err = ''
        if not test_with_args(exename, args, out, err, bool(err)):
            return False
    return True

def test_running_hexdump_stdin(exename):
    for filename in hd_filenames:
        if filename == '127.dat':
            continue
        filepath = os.path.join('tests', filename)
        args = ['--hexdump']
        with open(filepath + '.txt') as expected_file:
            expected_output = expected_file.read()
        with open(filepath, "rb") as inputfile:
            bytes_input = inputfile.read()
        if not test_with_args(exename, args, expected_output, '', 0, bytes_input):
            return False
    return True



def test_with_args(exename, args,
        out_expected, err_expected, ret_expected, stdin=None):
    '''Test running the program with args and verify expected output'''
    out, err, ret = run_with_args(args, exename, stdin=stdin)
    if ( (not err_expected and out_expected != out)
      or err_expected != err
      or ret_expected != (ret & 1)):
        msg = 'stdout, stderr or return code incorrect.'
        expected = pretty_run_with_args(exename, args, out_expected,
                err_expected, ret_expected)
        found = pretty_run_with_args(exename, args, out, err, ret)
        failed_test_message(msg, expected, found)
        return False

    return True

#
# Need to give a good error message so that students
# can understand...
#

FAILED_MESSAGE = '''\

Error: %s

I expected to see this:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
%s
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
But what I actually found is this:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
%s
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'''
def failed_test_message(msg, expected, found):
    msg = FAILED_MESSAGE % (msg, expected, found)
    print(msg, file=sys.stderr)

#
# Subprocess running functions
#

def pretty_run_with_args(exename, args, out, err, ret):
    '''Format running program with args and seeing out, err and ret'''
    session_fmt = '''$ %s%s%s\n(return): %d'''
    cmdline = ' '.join([exename] + args)
    if out:
        out = ('\n' + out).replace('\n', '\n(stdout): ')
    if err:
        err = ('\n' + err).replace('\n', '\n(stderr): ')
    outlines = ''.join('\n(stdout): ' + line for line in out.splitlines())
    errlines = ''.join('\n(stderr): ' + line for line in err.splitlines())
    return session_fmt % (cmdline, out, err, ret)


def run_subprocess(cmdline, stdin=None):
    '''Run command and return stdout, stderr and return code'''
    if stdin is None:
        process = subprocess.Popen(cmdline,
                stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        stdout, stderr = process.communicate()
    else:
        process = subprocess.Popen(cmdline, stdin=subprocess.PIPE,
                stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        stdout, stderr = process.communicate(input=stdin)
    # Should work on Py 2 and 3
    stdout = stdout.decode('ascii', errors='replace').replace('\r\n', '\n')
    stderr = stderr.decode('ascii', errors='replace').replace('\r\n', '\n')
    return stdout, stderr, process.returncode

def run_with_args(args, exename, stdin=None):
    '''Run the student's program with args. Return out, err, ret'''
    cmdline = [exename] + args
    return run_subprocess(cmdline, stdin=stdin)

if __name__ == "__main__":
    sys.exit(main(*sys.argv[1:]))

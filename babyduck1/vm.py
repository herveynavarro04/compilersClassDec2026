import sys

GLOBAL = "global"
LOCAL = "local"
TEMP = "temp"
CONST = "const"
INT = "int"
FLOAT = "float"
BOOL = "bool"
STRING = "string"
RANGES = [
    (1000, 1999, GLOBAL, INT),
    (2000, 2999, GLOBAL, FLOAT),
    (3000, 3999, LOCAL, INT),
    (4000, 4999, LOCAL, FLOAT),
    (5000, 5999, TEMP, INT),
    (6000, 6999, TEMP, FLOAT),
    (7000, 7999, TEMP, BOOL),
    (8000, 8999, CONST, INT),
    (9000, 9999, CONST, FLOAT),
    (10000, 10999, CONST, STRING)
]


class BabyDuckRuntimeError(Exception):
    pass


def decode(addr):
    for start, end, segment, type_ in RANGES:
        if start <= addr <= end:
            return segment, type_, addr - start
    raise BabyDuckRuntimeError(f"invalid address {addr}")


class FuncRecord:
    def __init__(self, fields):
        self.name = fields[0]
        self.start_quad = int(fields[1])
        self.num_params = int(fields[2])
        self.local_vars = {INT: int(fields[3]), FLOAT: int(fields[4])}
        self.temp_vars = {INT: int(fields[5]), FLOAT: int(fields[6]), BOOL: int(fields[7])}
        self.return_vars = int(fields[8])
        self.param_addresses = [int(a) for a in fields[9:]]


def load_obj(path):
    with open(path) as f:
        lines = [line.rstrip("\n") for line in f]
    i = 0
    _, nums_int, nums_float = lines[i].split()
    global_size = {INT: int(nums_int), FLOAT: int(nums_float)}
    i += 1

    constants = {}
    count = int(lines[i].split()[1])
    i += 1

    for _ in range(count):
        addr_text, value = lines[i].split(" ", 1)
        addr = int(addr_text)
        _, type_, _ = decode(addr)
        if type_ == INT:
            constants[addr] = int(value)
        elif type_ == FLOAT:
            constants[addr] = float(value)
        else:
            constants[addr] = value[1:-1]
        i += 1

    functions = {}
    main_record = None
    count = int(lines[i].split()[1])
    i += 1
    for _ in range(count):
        record = FuncRecord(lines[i].split())
        if record.name == "main":
            main_record = record
        else:
            functions[record.start_quad] = record
        i += 1

    quadruples = []
    count = int(lines[i].split()[1])
    i += 1

    for _ in range(count):
        op, left, right, result = lines[i].split()
        quadruples.append((op, int(left), int(right), int(result)))
        i += 1

    return global_size, constants, functions, main_record, quadruples

class Frame:
    def __init__(self, record):
        self.record = record
        self.locals = {t: [None] * n for t, n in record.local_vars.items()}
        self.temps = {t: [None] * n for t, n in record.temp_vars.items()}
        self.return_ip = None

class Memory:
    def __init__(self, global_size, constants, main_record):
        self.globals = {t: [None] * n for t, n in global_size.items()}
        self.constants = constants
        self.call_stack = [Frame(main_record)]
        self.pending_frames = []

    def active_frames(self):
        return self.call_stack[-1]

    def _cell(self, segment, frame):
        if segment == GLOBAL:
            return self.globals
        if segment == LOCAL:
            return frame.locals
        return frame.temps

    def read(self, addr):
        segment, type_, offset = decode(addr) 
        if segment == CONST:
            return self.constants[addr]
        
        value = self._cell(segment, self.active_frames())[type_][offset]
        if value is None:
            raise BabyDuckRuntimeError(f"variable at address {addr} used before assignment")
        return value

    def write(self, addr, value, frame=None):
        segment, type_, offset = decode(addr)
        if segment == CONST:
            raise BabyDuckRuntimeError(f"cannot write to constant address {addr}")
        if type_ == INT:
            value = int(value)
        elif type_ == FLOAT:
            value = float(value)
        self._cell(segment, frame or self.active_frames())[type_][offset] = value

ARITHMETIC = {
    "+": lambda a, b: a + b,
    "-": lambda a, b: a - b,
    "*": lambda a, b: a * b,
    "<": lambda a, b: a < b,
    ">": lambda a, b: a > b, 
    "!=": lambda a, b: a != b,
}

def format_value(value):
    if isinstance(value, bool):
        return "true" if value else "false"
    return str(value)

def run(path):
    global_size, constants, functions, main_record, quadruples = load_obj(path)
    memory = Memory(global_size, constants, main_record)
    line = []
    ip = 0

    def end_function():
        frame = memory.call_stack.pop()
        return frame.return_ip

    while True:
        op, left, rigth, result = quadruples[ip]
        next_ip = ip + 1

        if op in ARITHMETIC:
            memory.write(result, ARITHMETIC[op](memory.read(left), memory.read(rigth)))
        elif op == "/":
            divisor = memory.read(rigth)
            if divisor == 0:
                raise BabyDuckRuntimeError("division by zero")
            memory.write(result, memory.read(left) / divisor)
        elif op == "=":
            memory.write(result, memory.read(left))
        elif op == "print":
            line.append(format_value(memory.read(left)))
        elif op == "println":
            print("".join(line))
            line = []
        elif op == "read":
            text = input().strip()
            _, type_, _ = decode(result)
            try:
                value = int(text) if type_ == INT else float(text)
            except ValueError:
                raise BabyDuckRuntimeError(f"invalid {type_} input '{text}'")
            memory.write(result, value)
        elif op == "goto":
            next_ip = result
        elif op == "gotof":
            if not memory.read(left):
                next_ip = result
        elif op == "era":
            memory.pending_frames.append(Frame(functions[result]))
        elif op == "param":
            frame = memory.pending_frames[-1]
            target = frame.record.param_addresses[result - 1]
            memory.write(target, memory.read(left), frame)
        elif op == "gosub":
            frame = memory.pending_frames.pop()
            frame.return_ip = ip + 1
            memory.call_stack.append(frame)
            next_ip = result
        elif op == "return":
            memory.write(result, memory.read(left))
            next_ip = end_function()
        elif op == "endfunc":
            next_ip = end_function()
        elif op == "end":
            break
        else:
            raise BabyDuckRuntimeError(f"unknown operation '{op}'")
        
        ip = next_ip
            
def main():
    if len(sys.argv) != 2:
        print("usage: python3 vm.py <program.obj>", file=sys.stderr)
        sys.exit(3)
    try:
        run(sys.argv[1])
    except FileNotFoundError:
        print(f"file not found: {sys.argv[1]}", file=sys.stderr)
        sys.exit(3)
    except BabyDuckRuntimeError as error:
        print(f"Runtime error: {error}", file=sys.stderr)
        sys.exit(6)


if __name__ == "__main__":
    main()
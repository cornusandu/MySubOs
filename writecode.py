import struct


REGTOID = {
    'r0': b'\x00',
    'r1': b'\x01',
    'r2': b'\x02',
    'r3': b'\x03',
    'r4': b'\x04',
    'r5': b'\x05',
    'v0': b'\x06'
}

def isregister(a):
    return a in {'r0', 'r1', 'r2', 'r3', 'r4', 'r5', 'v0'}

binary = b''
for line in lines:
    if line[0] == 'halt':
        binary += b'\x00'
    elif line[0] == 'nop':
        binary += b'\x01'
    elif line[0] == 'mov':
        token1, token2 = line[1], line[2]
        if token2 in {'r0', 'r1', 'r2', 'r3', 'r4', 'r5', 'v0'}:
            binary += b'\x03' + REGTOID[token1] + REGTOID[token2]
        elif token2.isdigit():
            binary += b'\x02' + REGTOID[token1] + struct.pack('>H', int(token2))
    elif line[0] == 'add':
        token1, token2, token3 = line[1], line[2], line[3]
        if token2 in {'r0', 'r1', 'r2', 'r3', 'r4', 'r5', 'v0'} and token3 in {'r0', 'r1', 'r2', 'r3', 'r4', 'r5', 'v0'}:
            binary += b'\x04' + REGTOID[token1] + REGTOID[token2] + REGTOID[token3]
        elif token2 in {'r0', 'r1', 'r2', 'r3', 'r4', 'r5', 'v0'}:
            binary += b'\x05' + REGTOID[token1] + REGTOID[token2] + struct.pack('>H', int(token3))
        elif token3 in {'r0', 'r1', 'r2', 'r3', 'r4', 'r5', 'v0'}:
            binary += b'\x05' + REGTOID[token1] + REGTOID[token3] + struct.pack('>H', int(token2))
    elif line[0] == 'str':
        token1, token2 = line[1], line[2]
        if isregister(token1) and token2.isdigit():
            binary += b'\x06' + REGTOID[token1] + struct.pack('>H', int(token2))
        elif isregister(token2) and token1.isdigit():
            binary += b'\x07' + REGTOID[token2] + struct.pack('>H', int(token1))
        else:
            binary += b'\x08' + struct.pack('>H', int(token1)) + struct.pack('>H', int(token2))



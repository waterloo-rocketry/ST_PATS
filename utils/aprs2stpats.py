import sys
import socket
import time
import aprspy

HOST = '127.0.0.1'
PORT = 8001
CALLSIGN = 'VE3MYO-1'
OUTPUT = sys.stdout
DEBUG = False

def parse_aprs(data):
    def parse_callsign(data):
        return ''.join(chr(c >> 1) for c in data[:6]).strip() + '-' + str(data[6] >> 1 & 0xF)

    addr, info = data.strip(b'\r\n\xC0\x00').split(b'\x03\xF0')
    des = parse_callsign(addr)
    src = parse_callsign(addr[7:])
    paths = [parse_callsign(addr[i:]) for i in range(14, len(addr), 7)]
    info = info.decode('utf-8', errors='ignore')
    if len(paths) == 0:
        paths = ['NOPATH']
    frame = f"{src}>{des},{','.join(paths)}:{info}"

    if DEBUG:
        sys.stderr.write(frame + '\n')

    try:
        return aprspy.APRS.parse(frame)
    except (aprspy.exceptions.ParseError, aprspy.exceptions.UnsupportedError) as e:
        if DEBUG:
            print(e)
        return None

def parse_args():
    global HOST, PORT, CALLSIGN, OUTPUT, DEBUG
    def help_and_exit():
        print(f'{sys.argv[0]} [--host HOST | -H HOST] [--port PORT | -p PORT] [--callsign CALLSIGN | -c CALLSIGN] [--output OUTPUT | -o OUTPUT] [--debug]')
        sys.exit()

    try:
        while len(sys.argv) > 1:
            arg = sys.argv.pop(1)
            if arg == '--help' or arg == '-h':
                help_and_exit()
            elif arg == '--host' or arg == '-H':
                HOST = sys.argv.pop(1)
            elif arg == '--port' or arg == '-p':
                PORT = int(sys.argv.pop(1))
            elif arg == '--callsign' or arg == '-c':
                CALLSIGN = sys.argv.pop(1)
            elif arg == '--output' or arg == '-o':
                file = sys.argv.pop(1)
                if file != '-':
                    OUTPUT = open(file, 'w')
            elif arg == '--debug':
                DEBUG = True
    except (IndexError, ValueError):
        help_and_exit()

def main():
    parse_args()

    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((HOST, PORT))
    s.setblocking(False)

    try:
        while True:
            frame = None
            try:
                frame = parse_aprs(s.recv(1024))
            except BlockingIOError:
                time.sleep(0.2)

            if (
                frame is not None and
                #frame.source == CALLSIGN and
                isinstance(frame, aprspy.PositionPacket)
            ):
                if frame.latitude:
                    OUTPUT.write(f'{frame.latitude}N')
                if frame.longitude:
                    OUTPUT.write(f'{frame.longitude}E')
                if frame.altitude:
                    OUTPUT.write(f'{frame.altitude}M')
                OUTPUT.flush()

    except KeyboardInterrupt:
        pass

    s.close()
    if OUTPUT != sys.stdout:
        OUTPUT.close()

if __name__ == '__main__':
    main()

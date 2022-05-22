import sys
import socket
import time
import argparse
import aprspy

args = None

# parse APRS AX.25 KISS frames into aprspy classes, adopted from
def parse_aprs(data):
    def parse_callsign(data):
        # parses AX.25 callsign, see section 3.12 of http://ax25.net/AX25.2.2-Jul%2098-2.pdf
        return ''.join(chr(c >> 1) for c in data[:6]).strip() + '-' + str(data[6] >> 1 & 0xF)

    # strip KISS frame, and split the AX.25 frame into address and information
    # 0x03 = UI frame, 0xF0 = no layer 3 protocol
    # wrong format will get rejected by APRS.parse later
    addr, info = data.strip(b'\r\n\xC0\x00').split(b'\x03\xF0')
    des = parse_callsign(addr)
    src = parse_callsign(addr[7:])
    paths = [parse_callsign(addr[i:]) for i in range(14, len(addr), 7)]
    info = info.decode('utf-8', errors='ignore')

    if len(paths) == 0:
        paths = ['NOPATH']

    # combine callsigns and data into APRS format
    frame = f"{src}>{des},{','.join(paths)}:{info}"

    if args.debug:
        sys.stderr.write(frame + '\n')

    # try parse the combined APRS frame with aprspy
    try:
        return aprspy.APRS.parse(frame)
    except (aprspy.exceptions.ParseError, aprspy.exceptions.UnsupportedError) as e:
        if args.debug:
            sys.stderr.write(repr(e) + '\n')
        return None

def main():
    global args
    ap = argparse.ArgumentParser(
        description="Connect to KISS server and output GPS coordinate in St Pats format",
        formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    ap.add_argument('-H', '--host', type=str, default='127.0.0.1', help="Hostname of KISS server")
    ap.add_argument('-p', '--port', type=int, default=8001, help="Port of KISS server")
    ap.add_argument('-c', '--callsign', type=str, help="Only output messages destined to the callsign")
    ap.add_argument('-o', '--output', type=argparse.FileType('w'), default='-', help="File or serial port to output to")
    ap.add_argument('--debug', action='store_true')
    args = ap.parse_args()

    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((args.host, args.port))
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
                (args.callsign is None or frame.source == args.callsign) and
                isinstance(frame, aprspy.PositionPacket)
            ):
                if frame.latitude:
                    args.output.write(f'{frame.latitude}N')
                if frame.longitude:
                    args.output.write(f'{frame.longitude}E')
                if frame.altitude:
                    args.output.write(f'{frame.altitude}M')
                args.output.flush()

    except KeyboardInterrupt:
        pass

    s.close()
    if args.output != sys.stdout:
        args.output.close()

if __name__ == '__main__':
    main()

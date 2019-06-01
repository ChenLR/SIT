
class Protocal(object):
    """
    0x0D 0x0A length data_0 data_1 ... sum (sum starts from length)
    """
    def __init__(self, package_handler=None):
        self.max_package_length = 20
        self.__max_frame_length = 45
        self.__first_byte = 0x0D
        self.__second_byte = 0x0A
        self.__pad_byte = 0x50
        # below used by recvByteHandler 
        self.__package_handler = package_handler if package_handler is not None else self.packageHandler
        self.__recvBuffer = []
        self.__first_byte_trigger = False
        self.__second_byte_trigger = False

        pass

    def recvByteHandler(self, byte):
        # TODO
        self.__recvBuffer.append(byte)
        if byte == self.__first_byte:
            self.__first_byte_trigger = True
            self.__second_byte_trigger = False

        elif byte == self.__second_byte:
            if self.__first_byte_trigger:
                self.__second_byte_trigger = True
                self.__first_byte_trigger = False





    def packageToFrame(self, package):
        if len(package) > self.max_package_length:
            raise ValueError('package is oversized')
        # check range
        for byte in package:
            if byte < 0 or byte >= 256:
                raise ValueError('byte in package is out of range, byte = {}'.format(byte))
        # padding
        package_padded = []
        for byte in package:
            package_padded.append(byte)
            if byte == self.__first_byte:
                package_padded.append(self.__pad_byte)
        # padd for the length
        if len(package_padded) == self.__first_byte:
            package_padded = [self.__pad_byte] + package_padded
        # add length
        frame = [len(package_padded)] + package_padded
        # add sum
        frame = frame + [sum(frame)%256]
        # add header
        frame = [self.__first_byte, self.__second_byte] + frame
        # check frame length
        if len(frame) > self.__max_frame_length:
            raise ValueError('frame is oversized')
        return frame

    def removePadding(self, package_padded):
        if len(package_padded) == self.__first_byte + 1:
            package_padded = package_padded[1:]
        package = []
        idx = 0
        while idx < len(package_padded):
            byte = package_padded[idx]
            package.append(byte)
            if byte == self.__first_byte:
                idx += 2
            else:
                idx += 1
        return package

    def isFrameLegal(self, frame):
        if len(frame) < 4:
            return False
        if not frame[0] == self.__first_byte:
            return False
        if not frame[1] == self.__second_byte:
            return False
        if not frame[2] == len(frame) - 4:
            return False
        if not frame[-1] == sum(frame[2:-1])%256:
            return False
        return True

    def printAsHex(self, frame):
        print('[{}]'.format(', '.join(hex(x) for x in frame)))


import serial
import serial.tools.list_ports

class booleanStatus(object):
    def __init__(self, val=False):
        self.__val__ = val
    
    def get(self):
        return self.__val__
    
    def set(self, val):
        self.__val__ = val

class PortManager(object):
    def __init__(self, status_intf=booleanStatus, warning_intf=None):
        # self.root = root
        self.port_list = []
        self.port_status_list = []
        self.ser = None
        self.ser_idx = -1
        self.status_intf = status_intf
        if warning_intf is not None:
            self.warning_intf = warning_intf
        else:
            self.warning_intf = self.showWarning
        self.refreshList()

    def disconnectAllPorts(self):
        if self.ser is not None:
            try:
                self.ser.close()
            except:
                self.warning_intf(title='Warning', message='Close port failed')
            if self.ser.isOpen():
                self.warning_intf(title='Warning', message='Port still open')
            self.ser = None
            self.ser_idx = -1

        for idx, port in enumerate(self.port_list):
            status = self.port_status_list[idx]
            if status.get():
                # disconnect this port
                status.set(False)

    def connectPort(self, idx):
        if self.ser is not None:
            return
        port = self.port_list[idx].device
        baudRate=9600
        try:
            ser = serial.Serial(port,baudRate)
        except: # port occupied by other program
            self.warning_intf(title='Warning', message='{} is occupied'.format(port))
            status = self.port_status_list[idx]
            status.set(False)
            return 0
        if ser.isOpen(): 
            self.ser = ser
            self.ser_idx = idx
            status = self.port_status_list[idx]
            status.set(True)
            return 1
        else:
            status = self.port_status_list[idx]
            status.set(False)
            self.warning_intf(title='Warning', message='Failed to open {}'.format(port))
            return 0

    def refreshList(self):
        port_list = list(serial.tools.list_ports.comports())
        self.port_list = [item for item in port_list if 'Serial' in item.description or "usbserial" in item.device]
        self.port_status_list = [self.status_intf() for _ in self.port_list]

    def sendMessage(self, message):
        if self.ser is not None and self.ser.isOpen():
            try:
                self.ser.write(message.encode())
            except:
                self.warning_intf(title='Warning', message='Transmission failed')
        else:
            self.warning_intf(title='Warning', message='Port is not open')
                
    def showWarning(self, title, message):
        print("Warning! Title: {}, Message: {}".format(title, message))

from pywinauto.application import Application, WindowSpecification
import pywinauto.mouse as mouse
import pywinauto.keyboard as keyboard


class IoControl:
    def __init__(self):
        self.app: Application = None
        self.dlg: WindowSpecification = None

    def connect(self, pid):
        self.app = Application().connect(process=pid)
        self.dlg = self.app["Audition"]

    def focus(self):
        self.dlg.set_focus()

    def get_app_region(self) -> tuple:
        rect = self.dlg.rectangle()
        return (rect.left, rect.top, rect.right, rect.bottom)

    def send_keys(self, keys):
        # keyboard.send_keys(keys)
        print(keys)

import logging

class Logger:

    def __init__(self, filename: str, name: str = 'DEBUG') -> None:
        
        self.logger = logging.getLogger(name = name)
        
        f_handler = logging.FileHandler(filename = filename)
        f_handler.setLevel(logging.DEBUG)

        self.logger.addHandler(f_handler)
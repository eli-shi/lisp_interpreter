class Animal:
    def __init__(self, name):
        self.name = name

    def get_name(self):
        return self.name


class Mammal(Animal):
    def __init__(self, name):
        super().__init__(name)
        self.does_live_birth = True

    def live_birth(self):
        if not self.does_live_birth:
            raise RuntimeError("This animal lays eggs!")
        return "cute baby mamal"


class Rabbit(Mammal):
    def __init__(self, name, fur_color):
        super().__init__(name)
        self.fur_color = fur_color

    def get_fur_color(self):
        return self.fur_color


class Platypus(Mammal):
    def __init__(self, name):
        super().__init__(name)
        self.does_live_birth = False


rabbit = Rabbit("Roger", "grey")
baby = rabbit.live_birth()
print(f"Look a {baby}!")

parry = Platypus("Parry")
baby = parry.live_birth()
print(f"Look a {baby}!")

# parry.does_live_birth = True

# print(parry.does_live_birth)

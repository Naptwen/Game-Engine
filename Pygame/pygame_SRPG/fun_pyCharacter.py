import random


magic_fire_ball = [1, 5, 0 , "cricle"]
magic_earth_quake = [1, 4, 1, "circle"]
magic_lightning_volt = [1,5,1, "line"]

class character(object):
    max_hp = 5
    max_hit = 1
    max_dep = 1
    max_spd = 2
    max_mov = 3
    name = "c1"
    hp = 5
    hit = 1
    atk = 2
    dep = 1
    spd = 2
    mag = 1
    mov = 3
    ran = 1
    img_c = "c1.png"
    img_f = "c1.png"
    tag = None
    Ai_type = "FOX"
    def __init__(self, name, hp, hit, atk, dep, spd, mag, mov, ran, img_c, img_f, tag, Ai_type):
        self.max_hp = hp
        self.max_hit = hit
        self.max_atk = atk
        self.max_dep = dep
        self.max_spd = spd
        self.max_mov = mov
        self.name = name
        self.img_c = img_c
        self.img_f = img_f
        self.tag = tag
        self.hp = hp
        self.hit = hit
        self.atk = atk
        self.dep = dep
        self.spd = spd
        self.mag = mag
        self.mov = mov
        self.ran = ran
        self.Ai_type = Ai_type

    def phase_end(self):
        self.mov = self.max_mov
        self.hit = self.max_hit

    def damage_percentage(self, opp):
        Cri, Hit, Miss = [0, 0, 0]
        if self.atk  >= opp.dep * 2:
            min = 8
        elif self.atk > opp.dep :
            min = 7
        elif self.atk == opp.dep:
            min = 6
        elif self.atk * 2 <= opp.dep:
            min = 4
        elif self.atk < opp.dep:
            min = 5
        Miss = 100 - min * 10
        Hit = min 
        if self.spd  >= opp.spd * 2:
            min = 5
        elif self.spd > opp.spd :
            min = 4
        elif self.spd == opp.spd:
            min = 3
        elif self.spd * 2 <= opp.spd:
            min = 1
        elif self.spd < opp.spd:
            min = 2
        
        Cri = Hit * min
        Hit = 100 - Miss - Cri
        return [Cri, Hit, Miss]

    def magic(self, opp):
        dice = random.randrange(1,self.mag)
        dice2 = random.randrange(1,opp.mag)
        if dice < dice2:
            return True

    def attack(self, opp):
        dice = random.randrange(1,10)
        if self.atk  >= opp.dep * 2:
            min = 8
        elif self.atk > opp.dep :
            min = 7
        elif self.atk == opp.dep:
            min = 6
        elif self.atk * 2 <= opp.dep:
            min = 4
        elif self.atk < opp.dep:
            min = 5
        if dice <= min: 
            dice = random.randrange(1,10)
            print("[ATTACK] : Hit")
            if self.spd  >= opp.spd * 2:
                min = 5
            elif self.spd > opp.spd :
                min = 4
            elif self.spd == opp.spd:
                min = 3
            elif self.spd * 2 <= opp.spd:
                min = 1
            elif self.spd < opp.spd:
                min = 2
            if dice <= min:
                opp.hp -= self.atk * 2
                return [2, self.atk * 2] 
            else:
                opp.hp -= self.atk
                return [1, self.atk] 
        else:
            print("[ATTACK] : miss")
            return [0, 0]

    def die_check(self):
        if self.hp <= 0:
            return 1

enemy_list = {"orc1": 1, "ogr1": 2, "troll1": 3}

enemy = { 
        1:character("orc1",5,1,2,1,1,1,3,1,"orc1.png","orc1.png","enemy", "FOX"),\
        2:character("ogr1",7,2,3,1,1,1,3,1,"ogr1.png","org1.png","enemy", "FOX"),\
        3:character("troll1",4,2,2,0,1,1,4,2,"troll1.png","troll1.png","enemy", "FOX")}

troop = {
        "Footman":character("Footman",5,1,2,1,1,1,3,1,"c1.png","c1.png","troop", "FOX"),\
        "Archer":character("Archer",5,1,2,1,2,1,3,2,"c2.png","c2.png","troop", "FOX"),\
        "Peasant":character("Peasant",4,2,1,0,1,1,3,1,"c3.png","c3.png","troop", "FOX")}
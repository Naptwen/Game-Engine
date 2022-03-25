import pygame
import fun_pyCell

def Ai_mag(AI_cell, Table):
    magic = Table.range(AI_cell.pos,AI_cell.type.int)
 


def Ai_atk(AI_cell, Table, future_pos, best_target_pos, AI_target_tag, factors):
    AI_order = []
    atk_path = []
    a, b, c, d, e, f = factors
    for i in range(0,AI_cell.type.hit):
        atk_path = Table.area(future_pos, AI_cell.type.ran, "line", True)
    target_list = {}
    for target in atk_path:
        target_cell = Table.giveCell(target)
        if target_cell.type is not None and target_cell.type.tag == AI_target_tag:
            if target_cell.pos == best_target_pos:
                target_list[100] = best_target_pos
                break
            else:
                point = float(target_cell.type.max_hp) * a + float(target_cell.type.hp/target_cell.type.max_hp) * b\
                     + float(target_cell.type.atk) * d + float(target_cell.type.ran) * e
                target_list[point] = target_cell.pos
    if target_list: 
        best_target_pos2 = target_list[min(target_list)]
        AI_order.append(["atk", best_target_pos2])
    return AI_order

def Ai_mov(AI_cell,Table,best_target_pos, keep_distance):
    AI_order = []
    foot_step = AI_cell.type.mov - 1
    AI_pos = AI_cell.pos
    if foot_step > 0:
        mov_path = Table.Heuristics_path(AI_pos, best_target_pos)
        for pos in mov_path:
            AI_order.append(["mov", pos])
            AI_pos = pos
            foot_step -= 1
            if foot_step <= 0:
                break   
        if AI_cell.type.mov + AI_cell.type.ran >= len(mov_path) and keep_distance == True:
            for i in range(0, AI_cell.type.ran - 1):
                AI_order = AI_order[:-1]
    return AI_order
#recevie pos (not index)
def Ai_order(AI_pos, AI_target_tag, Table, AI_TYPE):
    AI_cell = Table.giveCell(AI_pos)
    AI_order = []
    a, b, c, d, e, f= [1,1,-3,0,0, True]
    if AI_TYPE == "FOX":
        a, b, c, d, e, f = [1,1,-10,1,1, True]
    elif AI_TYPE == "BEAR":
        a, b, c, d, e, f = [2,1,-2,4,1, True]
    elif AI_TYPE == "TUTLE":
        a, b, c, d, e, f = [2,-10,3,1,5, True]
    factors = [a,b,c,d,e,f]
    target_list = []
    for target in Table.node_table:
        if target.type is not None and target.type.tag == AI_target_tag:
            AI_path = Table.Heuristics_path(AI_pos, target.pos)
            way = len(AI_path) 
            if way == 0 and target.pos[0] - AI_pos[0] + target.pos[1] - AI_pos[1] != 1:
                way = -1
            point = float(target.type.max_hp) * a + float(target.type.hp/target.type.max_hp) * b\
            + float(way) * c + float(target.type.atk) * d + float(target.type.ran) * e
            target_list.append([point, target.pos])
    if target_list: 
        max = target_list[0][0]
        max_index = 0
        for i, sublist in enumerate(target_list):
            if max < sublist[0]:
                max = sublist[0]
                max_index = i
        best_target_pos = target_list[max_index][1]
        AI_order.append(["target", best_target_pos])
        AI_order += Ai_mov(AI_cell, Table, best_target_pos, f)
        for i in range(0,AI_cell.type.hit):
            future_pos = AI_pos
            if AI_order[-1][0] == "mov":
                future_pos = AI_order[-1][1]
            AI_order += Ai_atk(AI_cell, Table, future_pos, best_target_pos, AI_target_tag, factors)
    return AI_order

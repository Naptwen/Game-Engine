import numpy as np
import copy

def merge(map, map_size, paint, img, img_map):
    img_map = copy.deepcopy(img_map)
    for i in range(0, map_size[0]):
        for j in range(0, map_size[1]):
            if map[i][j] == paint:
                left = False
                right = False
                up = False
                down = False
                if i - 1 >= 0:
                    if map[i - 1, j] == paint:
                        left = True
                if j - 1 >= 0:
                    if map[i, j - 1] == paint:
                        up = True
                if i + 1 < map_size[0]:
                    if map[i + 1, j] == paint:
                        right = True
                if j + 1 < map_size[1]:
                    if map[i, j + 1] == paint:
                        down = True
                direction = [left, right, up, down]
                if direction == [0, 0, 0, 0]:  # none
                    img_map[i][j] = img[0]
                elif direction == [1, 0, 0, 0]:  # left
                    img_map[i][j] = img[1]
                elif direction == [0, 1, 0, 0]:  # right
                    img_map[i][j] = img[2]
                elif direction == [0, 0, 1, 0]:  # up
                    img_map[i][j] = img[3]
                elif direction == [0, 0, 0, 1]:  # down
                    img_map[i][j] = img[4]
                elif direction == [1, 1, 0, 0]:  # -
                    img_map[i][j] = img[5]
                elif direction == [0, 0, 1, 1]:  # |
                    img_map[i][j] = img[6]
                elif direction == [1, 0, 1, 0]:  # ┛
                    img_map[i][j] = img[7]
                elif direction == [1, 0, 0, 1]:  # ┓
                    img_map[i][j] = img[8]
                elif direction == [0, 1, 1, 0]:  # ┗
                    img_map[i][j] = img[9]
                elif direction == [0, 1, 0, 1]:  # ┏
                    img_map[i][j] = img[10]
                elif direction == [1, 1, 1, 0]:  # ┻
                    img_map[i][j] = img[11]
                elif direction == [1, 1, 0, 1]:  # ┳
                    img_map[i][j] = img[12]
                elif direction == [1, 0, 1, 1]:  # ┨
                    img_map[i][j] = img[13]
                elif direction == [0, 1, 1, 1]:  # ┝
                    img_map[i][j] = img[14]
                elif direction == [1, 1, 1, 1]:  # ╋
                    img_map[i][j] = img[15]
    return img_map

def painting(tile_map, tile_map_size, cell_pos, bucket, paint):
    if bucket == False:
        tile_map[cell_pos[0]][cell_pos[1]] = paint
    else:
        if tile_map[cell_pos[0]][cell_pos[1]] != paint:
            open_list = []
            open_list.append([cell_pos[0], cell_pos[1]])
            orin_paint = tile_map[cell_pos[0]][cell_pos[1]]
            while open_list:
                temp = open_list.pop()
                tile_map[temp[0]][temp[1]] = paint
                if temp[0] - 1 >= 0:
                    if tile_map[temp[0] - 1, temp[1]] == orin_paint:
                        open_list.append([temp[0] - 1, temp[1]])
                if temp[1] - 1 >= 0:
                    if tile_map[temp[0], temp[1] - 1] == orin_paint:
                        open_list.append([temp[0], temp[1] - 1])
                if temp[0] + 1 < tile_map_size[0]:
                    if tile_map[temp[0] + 1, temp[1]] == orin_paint:
                        open_list.append([temp[0] + 1, temp[1]])
                if temp[1] + 1 < tile_map_size[1]:
                    if tile_map[temp[0], temp[1] + 1] == orin_paint:
                        open_list.append([temp[0], temp[1] + 1])

class NODE(object):
    pos = []
    neigh = []
    price = -1  # -1 means no prcie marked
    parent = -1  # -1 means no parent node marked
    dis = 1  # the connection between nodes
    check = False  # for checking
    obstacle = False
    block = False
    type = None
    status = None

class Cell_Table:
    rows = 0
    cols = 0
    node_table = []

    def __init__(self, cols, rows):
        self.node_table = []
        for j in range(0, rows):
            for i in range(0, cols):
                __node = NODE()
                __node.pos = [i, j]
                __node.neigh = []
                if j - 1 >= 0:
                    __node.neigh.append((j - 1) * cols + i)
                if j + 1 < rows:
                    __node.neigh.append((j + 1) * cols + i)
                if i - 1 >= 0:
                    __node.neigh.append(j * cols + (i - 1))
                if i + 1 < cols:
                    __node.neigh.append(j * cols + (i + 1))
                __node.dis = 1
                __node.price = -1
                __node.parent = -1
                __node.check = False
                __node.obstacle = False
                __node.block = False
                self.node_table.append(__node)
        self.rows = rows
        self.cols = cols

    def giveCell(self, cell):
        if cell:
            if self.cols > cell[0] >= 0  and self.rows > cell[1] >= 0:
                if len(self.node_table) > self.cols * cell[1] + cell[0] >= 0:
                    return self.node_table[self.cols * cell[1] + cell[0]]
        return None
        
    def Astar(self, start, end, block):
        self.clearCell()
        start_index = self.cols * start[1] + start[0]  # convert coordinates in index
        end_index = self.cols * end[1] + end[0]  # convert coordinates in index
        find = False
        parent = start_index
        if end[1] > self.rows or end[0] > self.cols or start[1] > self.rows or start[0] > self.cols or end[1] < 0 or end[0] < 0 or start[1] < 0 or start[0] < 0:
            return []
        elif start_index == end_index:
            return []
        else:
            open_que = {}
            G = self.node_table[start_index].price + self.node_table[start_index].dis
            H = (self.node_table[start_index].pos[0] - self.node_table[end_index].pos[0])**2\
                + (self.node_table[start_index].pos[1] - self.node_table[end_index].pos[1])**2
            F = G + H
            open_que[start_index] = F
            while open_que:
                min_key = min(open_que, key = open_que.get)
                open_que.pop(min_key)
                parent = min_key
                self.node_table[parent].check = True
                if parent == end_index:
                    find = True
                    break
                neigh = self.node_table[parent].neigh
                for child in neigh:                  
                    if end_index == child:
                        self.node_table[child].parent = parent
                        open_que[child] = 0
                        break     
                    if self.node_table[child].obstacle == False\
                        and self.node_table[child].check == False\
                        and self.node_table[child].block != block:
                        G = self.node_table[parent].price + self.node_table[child].dis
                        if not child in open_que:
                            H = abs(self.node_table[child].pos[0] - self.node_table[end_index].pos[0])\
                                + abs(self.node_table[child].pos[1] - self.node_table[end_index].pos[1])
                            F = G + H
                            self.node_table[child].price = G
                            self.node_table[child].parent = parent
                            open_que[child] = F
                        else:                            
                            if child in open_que and G < self.node_table[child].price:
                                H = abs(self.node_table[child].pos[0] - self.node_table[end_index].pos[0])\
                                + abs(self.node_table[child].pos[1] - self.node_table[end_index].pos[1])
                                F = G + H
                                self.node_table[child].price = G
                                self.node_table[child].parent = parent
                                open_que[child] = F
            if find is True:
                way_list = []
                back = end_index
                turn = 0
                while True:
                    turn += 1  
                    way_list.append(self.node_table[back].pos)
                    back = self.node_table[back].parent
                    if back == start_index:
                        way_list.append(self.node_table[start_index].pos)
                        break
                reverse_list = way_list[::-1]
                del reverse_list[0]
                if self.node_table[end_index].block == True:
                    del reverse_list[-1]
                return reverse_list
            else:
                return []        

    def Dijkstar(self, start, end):
        self.clearCell()
        que = []
        start_index = self.cols * start[1] + start[0]  # convert coordinates in index
        end_index = self.cols * end[1] + end[0]  # convert coordinates in index
        if end[1] > self.rows or end[0] > self.cols or start[1] > self.rows or start[0] > self.cols or end[1] < 0 or end[0] < 0 or start[1] < 0 or start[0] < 0:
            return []
        else:
            que.append(start_index)
            # finding connected nodes
            trial = 0
            find = False
            _set = False
            if self.node_table[end_index].block:
                self.node_table[end_index].block = False
                _set = True
            while len(que) > 0 and trial < 99999999:
                trial = trial + 1
                parent = que.pop(0)  # pop the first index
                if parent == end_index:
                    find = True
                    break
                # find the neighbor indices
                neigh = self.node_table[parent].neigh
                for child in neigh:
                    # total price
                    new_price = self.node_table[child].dis + \
                        self.node_table[parent].price
                    # if price is less than given
                    if self.node_table[child].price == -1 or new_price < self.node_table[child].price:
                        # change price
                        self.node_table[child].price = new_price
                        # change its parent
                        self.node_table[child].parent = parent
                    if self.node_table[child].check is False and self.node_table[child].obstacle is False \
                        and self.node_table[child].block is False:
                        que.append(child)
                    self.node_table[child].check = True
            if _set:  # get back the block status
                self.node_table[end_index].block = True
            if find is True:
                way_list = []
                back = end_index
                way_list.append(self.node_table[back].pos)
                while back != start_index:  # until end at the start_index
                    back = self.node_table[back].parent
                    # node list of optimazation path in coordinates
                    way_list.append(self.node_table[back].pos)
                reverse_list = []
                if _set and way_list:  # since the end is block, erase the end point
                    way_list.pop(0)
                for way in way_list[::-1]:
                    reverse_list.append(way)
                return reverse_list
            else:
                return []

    def BFS(self, start, end):
            self.clearCell()
            start_index = self.cols * start[1] + start[0]  # convert coordinates in index
            end_index = self.cols * end[1] + end[0]  # convert coordinates in index
            if end[1] > self.rows or end[0] > self.cols or start[1] > self.rows or start[0] > self.cols or end[1] < 0 or end[0] < 0 or start[1] < 0 or start[0] < 0:
                return []     
            else:
                self.node_table[start_index].price = 99999999999999999999
                que = []
                que.append(start_index)
                while que:
                    parent = que.pop(0)
                    if parent == end_index:
                        break
                    neigh = self.node_table[parent].neigh
                    for child in neigh:
                        child_cell = self.node_table[child]
                        if child_cell.parent == -1:
                            child_cell.parent = parent
                            que.append(child)
            return_list = []
            temp =  end_index
            if self.node_table[temp].parent == -1:
                return[]
            while temp != -1:
                return_list.append(temp)
                temp = self.node_table[temp].parent
            return return_list

    def area(self, start, range, shape, obs):
        self.clearCell()
        que = []
        if shape == "circle":
            open_list = []
            start_index = self.cols * start[1] + start[0]
            start_cell = self.node_table[start_index]
            open_list.append(start_cell)
            while open_list:
                start_cell = open_list.pop(0)
                start_cell.check = True
                neigh = start_cell.neigh
                for ng in neigh:
                    temp = self.node_table[ng]
                    if temp.check == False and temp.obstacle != obs:
                        start_index = self.cols * \
                            start_cell.pos[1] + start_cell.pos[0]
                        temp.parent = start_index
                        temp.dis += self.node_table[start_index].dis
                        temp.check = True
                        if temp.dis <= range + 1:
                            open_list.append(temp)
                            que.append(temp.pos)
        elif shape == "line":
            open_list = []
            start_index = self.cols * start[1] + start[0]
            start_cell = self.node_table[start_index]
            open_list.append(start_cell)
            while open_list:
                start_cell = open_list.pop(0)
                start_cell.check = True
                neigh = start_cell.neigh
                for ng in neigh:
                    temp = self.node_table[ng]
                    if temp.check == False and temp.obstacle == False\
                        and (temp.pos[0] == start[0] or temp.pos[1] == start[1]):
                        start_index = self.cols * \
                            start_cell.pos[1] + start_cell.pos[0]
                        temp.parent = start_index
                        temp.dis += self.node_table[start_index].dis
                        temp.check = True
                        if temp.dis <= range + 1:
                            open_list.append(temp)
                            que.append(temp.pos)
        else:
            for node in self.node_table:
                if (node.pos[0] - start[0])**2 + (node.pos[1] - start[1])**2 < range**2:
                    que.append(node.pos)
        return que

    def water(self, start, range):
        self.clearCell()
        que = []
        open_list = []
        start_index = self.cols * start[1] + start[0]
        start_cell = self.node_table[start_index]
        que.append(start)
        open_list.append(start_cell)
        while open_list:
            start_cell = open_list.pop(0)
            start_cell.check = True
            neigh = start_cell.neigh
            for ng in neigh:
                temp = self.node_table[ng]
                if temp.check == False:
                    start_index = self.cols * \
                        start_cell.pos[1] + start_cell.pos[0]
                    temp.parent = start_index
                    temp.dis += self.node_table[start_index].dis
                    temp.check = True
                    if temp.dis <= range + 1:
                        open_list.append(temp)
                        que.append(temp.pos)
        return que

    def Heuristics_path(self, start, end):
        path = self.Astar(start, end, True)
        if path == []:
            path = self.Astar(start, end, None)
            path2 = []
            for temp in path:
                cell = self.giveCell(temp)
                if cell.block == True:
                    break
                else:
                    path2.append(cell.pos)
            return path2
        else:
            return path

    def checkObstacle(self, x, y):
        index = self.cols * y + x
        return self.node_table[index].obstacle

    def setObstacle(self, bool, x, y):
        index = self.cols * y + x
        self.node_table[index].obstacle = bool
    #receie pos of cell [not index]
    def swap_cell(self, A_pos, B_pos):
        A = self.cols * A_pos[1] + A_pos[0]
        B = self.cols * B_pos[1] + B_pos[0]
        pA = self.node_table[A]
        pB = self.node_table[B]
        temp_block = copy.deepcopy(pA.block)
        temp_type = copy.deepcopy(pA.type)
        pA.block = copy.deepcopy(pB.block)
        pA.type = copy.deepcopy(pB.type)
        pB.block = copy.deepcopy(temp_block)
        pB.type = copy.deepcopy(temp_type)

    def resetCell(self):
        for node in self.node_table:
            node.price = -1
            node.parent = -1
            node.dis = 1
            node.status = None
            node.check = False
            node.obstacle = False
            node.type = None
            node.block = False

    def eight_dir_check(self, cell_pos, type, block, obstacle):
        check_list = []
        pos_list = [[0,-1],[1,-1],[1,0],[1,1],[0,1],[-1,1],[-1,0],[-1,-1]]
        for i in range(0, 8):
            ng_pos = [cell_pos[0] - pos_list[i][0], cell_pos[1] - pos_list[i][1]]
            temp = self.node_table[self.cols * ng_pos[1] + ng_pos[0]]
            if temp is not None and temp.type == type and\
                temp.block != block and temp.obstacle != obstacle:
                check_list.append(ng_pos)  
        return check_list

    def clearCell(self):
        for node in self.node_table:
            node.price = -1
            node.parent = -1
            node.dis = 1
            node.check = False

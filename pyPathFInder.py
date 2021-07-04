import numpy as np

class NODE:
    pos = []
    neigh = []
    price = -1 #-1 means no prcie marked
    parent = -1 #-1 means no parent node marked
    dis = 1 #the connection between nodes
    check = False #for checking
    obstacle = False

class Cell_Table:
    rows = 0
    cols = 0
    node_table = []

    def __init__(self, rows, cols):
        for i in range(0, rows):
            for j in range(0, cols):
                __node = NODE()
                __node.pos = [i, j]
                __node.neigh = []
                if i - 1 >= 0 :
                    __node.neigh.append((i - 1) * cols + j)
                if i + 1 < rows :
                    __node.neigh.append((i + 1) * cols + j)
                if j - 1 >= 0 :
                    __node.neigh.append(i * cols + (j - 1))
                if j + 1 < cols:
                    __node.neigh.append(i * cols + (j + 1))
                __node.dis = 1
                __node.price = -1
                __node.parent = -1
                __node.check = False
                __node.obstacle = False
                self.node_table.append(__node)
        self.rows = rows
        self.cols = cols

    def Dijkstra(self, start, end):
        que = []
        start_index = self.cols * start[0] + start[1] #convert coordinates in index
        end_index = self.cols * end[0] + end[1] #convert coordinates in index
        if end[0] > self.rows or end[1] > self.cols or start[0] > self.rows or start[1] > self.cols or end[0] < 0 or end[1] < 0 or start[0] < 0 or start[1] < 0:
            print('finding is overflower')
            return []
        else:
            que.append(start_index)
            # finding connected nodes
            trial = 0
            find = False
            while len(que) > 0:
                trial = trial + 1
                parent = que.pop(0)  # pop the first index
                if parent == end_index:
                    find = True
                    break
                neigh = self.node_table[parent].neigh  # find the neighbor indices
                for child in neigh:
                    new_price = self.node_table[child].dis + self.node_table[parent].price  # total price
                    if self.node_table[child].price == -1 or new_price < self.node_table[child].price:  # if price is less than given
                        self.node_table[child].price = new_price  # change price
                        self.node_table[child].parent = parent  # change its parent
                    if self.node_table[child].check is False and self.node_table[child].obstacle is False:
                        que.append(child)
                    self.node_table[child].check = True
            if find is True:
                way_list = []
                back = end_index
                way_list.append(self.node_table[back].pos)
                while back != start_index: #until end at the start_index
                    back = self.node_table[back].parent
                    way_list.append(self.node_table[back].pos) #node list of optimazation path in coordinates
                return way_list
            else:
                print('finding is failed')
                return []

    def checkObstacle(self, x, y):
        index = self.cols * x + y
        return self.node_table[index].obstacle

    def setObstacle(self, bool, x, y):
        index = self.cols * x + y
        self.node_table[index].obstacle = bool

    def resetCell(self):
        for node in self.node_table:
            node.price = -1
            node.parent = -1
            node.check = False
            node.obstacle = False

    def clearCell(self):
        for node in self.node_table:
            node.price = -1
            node.parent = -1
            node.check = False





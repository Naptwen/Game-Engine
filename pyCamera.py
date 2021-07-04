import math
import numpy as np

def unit_vec(vec):
    len = math.sqrt(vec[0] ** 2 + vec[1] ** 2 + vec[2] ** 2)
    if len != 0:
        nor = np.array([vec[0] / len, vec[1] / len, vec[2] / len])
    else:
        nor = np.array([0, 0, 0])
    return nor


def camera_mov(camera, eye, x, y):
    vec = eye - camera
    up = np.array([0, 0, 1])
    hor = unit_vec(np.cross(vec, up))
    front = unit_vec(np.cross(up, hor))
    eye = eye + hor * x
    camera = camera + hor * x
    eye = eye + front * y
    camera = camera + front * y
    return [camera, eye]

def camera_rot_xy(camera, eye, r):
    vec = eye - camera
    r = math.radians(r)
    _x = math.cos(r) * -vec[0] - math.sin(r) * -vec[1]
    _y = math.sin(r) * -vec[0] + math.cos(r) * -vec[1]
    new_vec = np.array([_x + eye[0], _y + eye[1], camera[2]])
    return new_vec

def camera2world(camera, eye, size_camera, size_eye, position, zoom):
    w, h = [size_eye[0], size_eye[1]]
    ew, eh = [1.5, 2] # [size_eye[0], size_eye[1]]
    vec = camera - eye
    up = np.array([0, 0, 1])
    hort = unit_vec(np.cross(up, vec))
    vert = unit_vec(np.cross(vec, hort))
    uver = vert * eh / 2
    uhor = hort * ew / 2
    origin = camera - uver - uhor
    print(hort, vert, uver, uhor)
    print(eye, origin)
    depth = math.sqrt(vec[0] ** 2 + vec[1] ** 2 + vec[2] ** 2)
    x, y = [position[0], h - position[1]]
    r_x, r_y = [ew * x / w, eh * y / h]  # ratio of the position
    return origin + uver * r_y * zoom + uhor * r_x * zoom


import matplotlib.pyplot as plt
import matplotlib
from mpl_toolkits.mplot3d import Axes3D

class Point3D:
    def __init__(self):
        self.x = 0
        self.y = 0
        self.z = 0
        self.ts = 0
        self.ret = 0

    def __str__(self):
        return "%10d: ret=%2d %3d %3d %3d" % (self.ts, self.ret, self.x, self.y, self.z)


def parse_data(data_file):
    
    data_pts = []
    
    for line in data_file:

        line = line.strip()

        if not line:
            continue

        pt = Point3D()
        line = line.split(":") 
        
        ts = int(line[0].strip())
        
        line = line[1].split("]")
        coor = line[0].split("[")
        coor = coor[1].strip().split()
        
        x = int(coor[0])
        y = int(coor[1])
        z = int(coor[2])
        
        ret = line[1].strip().split("=")
        ret = int(ret[1])
        
        pt.ts = ts
        pt.x = x
        pt.y = y
        pt.z = z
        pt.ret = ret
        
        data_pts.append(pt)
        
    return data_pts
        
        
def main():

    data_file = open("data/mag_data9.txt", "r")
    data_pts = parse_data(data_file)

    x = []
    y = []
    z = []
    cnt = 0;
    num = len(data_pts)
    print(num)

    for pt in data_pts:
        cnt += 1
        
        # for 7    
        #if (cnt > 2400 or cnt < 200 ):
        #    continue
         
        # for 6    
        #if (cnt > 5300 or cnt < 500 ):
        #    continue
        
        x.append(pt.x)
        y.append(pt.y)
        z.append(pt.z)

    fig = plt.figure()
    ax = fig.add_subplot(111, projection="3d")
    ax.scatter(x, y, z, c=z, depthshade=True)

    ax.set_xlabel('X Label')
    ax.set_ylabel('Y Label')
    ax.set_zlabel('Z Label')
    #plt.colorbar()
    plt.draw()


main()

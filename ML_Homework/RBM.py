# -*- coding:utf-8 -*-
"""
Boltzmann机求解TSP问题
"""
import numpy as np
from numpy import *
import copy
import matplotlib.pyplot as plt
import pandas as pd
import xlrd

class BoltzmannNet(object):
    def __init__(self):
        self.dataMat = [] # 外部导入的数据集
        self.MAX_ITER = 2000  # 外循环迭代次数
        self.T0 = 1000        # 最高温度：这个温度的变化范围应位于最大迭代范围之内
        self.Lambda = 0.97    # 温度下降参数
        self.iteration = 0    # 迭代最优时的迭代次数
        self.dist = []        # 存储生成的距离
        self.pathindx = []    # 存储生成的路径索引
        self.bestdist = 0     # 生成的最优路径长度
        self.bestpath = []    # 生成的最优路径

    def loadDataSet(self,filename):
        self.dataMat = []
        fr = open(filename)
        for line in fr.readlines():
            lineArr = line.strip().split()
            self.dataMat.append([float(lineArr[0]), float(lineArr[1])])
        self.dataMat = mat(self.dataMat)
        return self.dataMat


    def distEclud(self, vecA, vecB):
        # 欧式距离
        distMat = zeros((vecA.shape[0], vecA.shape[0]))
        for i in range(len(vecA)):
            for j in range(len(vecA)):
                distMat[i][j] = linalg.norm(vecA[i] - vecB[:, j])
        return distMat


    def boltzmann(self,newl,oldl,T): # 玻尔兹曼函数
        return exp(-(newl-oldl)/T)


    def pathLen(self,dist,path): # 计算路径长度
        N = len(path)
        plen = 0
        for i in arange(0,N-1): # 长度为N的向量，包含1~N的整数
            plen += dist[path[i],path[i+1]]
        plen += dist[path[0],path[N-1]]
        return plen

    def changePath(self,old_path): # 交换新旧路径
        N = len(old_path)
        if np.random.rand() < 0.25: # 产生两个位置，并交换
            chpos = floor(np.random.rand(1,2)*N).tolist()[0]
            new_path = copy.deepcopy(old_path)
            new_path[int(chpos[0])] = old_path[int(chpos[1])]
            new_path[int(chpos[1])] = old_path[int(chpos[0])]
        else:  # 产生三个位置，交换a-b和b-c
            d = ceil(np.random.rand(1,3)*N).tolist()[0]
            d.sort() # 随机路径排序
            a = int(d[0])
            b = int(d[1])
            c = int(d[2])
            if a != b and b != c:
                new_path = copy.deepcopy(old_path)
                #new_path[a:c-1] = old_path[b-1:c-1]+ old_path[a:b-1] # python2 列表的某段替换
                new_path[a:a + c - b] = copy.copy(old_path[b - 1:c - 1]) # 与上等价
                new_path[a + c - b:c - 1] = copy.copy(old_path[a:b - 1])
                #new_path=new_path.tolist() # 数组转列表
            else:
                new_path = self.changePath(old_path)
        return new_path

    def drawPath(self,plt):# 绘制路径
        px = []
        py = []
        for Seq in self.bestpath:
            px.append(self.dataMat[Seq,0])
            py.append(self.dataMat[Seq,1])
        plt.plot(px,py,'b')


    def drawScatter(self,plt): # 绘制散点图
        px = (self.dataMat[:,0]).tolist()
        py = (self.dataMat[:,1]).tolist()
        plt.scatter(px,py,c='green',marker='o',s=60)
        i=65
        for x,y in zip(px,py):
            plt.annotate(str(chr(i)),xy=(x[0]+40,y[0]),color='black')
            i += 1

    def TrendLine(self,plt,color='b'): # 绘制趋势线
        plt.plot(arange(len(self.dist)),self.dist,color)

    def initBMNet(self,m,n,distMat): # 构造一个初始可行解
        self.pathindx = arange(m).tolist()
        np.random.shuffle(self.pathindx) # 随机生成每个路径
        self.dist.append(self.pathLen(distMat,self.pathindx)) # 每个路径对应的距离
        return self.T0,self.pathindx,m

    # 最短路径的实现
    # 1.导入数据，并根据配置参数初始化网络
    def train(self): # 主函数
        m,n = shape(self.dataMat)
        distMat = self.distEclud(self.dataMat,self.dataMat.T) # 转换为邻接矩阵（距离矩阵）
        # T为当前温度，curpath为当前路径索引，MAX_M为内循环最大迭代次数
        [T,curpath,MAX_M] = self.initBMNet(m,n,distMat)
        step = 0 # 初始化外循环迭代
        while step <= self.MAX_ITER: # 外循环
            m = 0 # 内循环计数器
            while m <= MAX_M:        # 内循环
                curdist = self.pathLen(distMat,curpath) # 计算当前路径距离
                newpath = self.changePath(curpath)      # 交换产生新路径
                newdist = self.pathLen(distMat,newpath) # 计算新路径距离
                # 判断网络是否是一个局部稳态
                if (curdist > newdist):
                    curpath = newpath
                    self.pathindx.append(curpath)
                    self.dist.append(newdist)
                    self.iteration += 1   # 增加迭代次数
                else: # 如果网络处于局部稳态，则执行玻尔兹曼函数
                    if np.random.rand() < self.boltzmann(newdist,curdist,T):
                        curpath = newpath
                        self.pathindx.append(curpath)
                        self.dist.append(newdist)
                        self.iteration += 1  # 增加迭代次数
                m += 1
            step += 1
            T = T*self.Lambda # 降温，返回迭代，直至算法终止

        # 提取最优路径
        self.bestdist = min(self.dist)
        indxes = argmin(self.dist)
        self.bestpath = self.pathindx[indxes]

if __name__ == "__main__":
    # 加载
    bmNet = BoltzmannNet()
    bmNet.loadDataSet("data.txt")
    bmNet.train()
    print("循环迭代",bmNet.iteration,"次")
    print("最优解：",bmNet.bestdist)
    print("最佳路线：",bmNet.bestpath)
    # 显示优化后的城市图，路径图
    bmNet.drawScatter(plt)
    bmNet.drawPath(plt)
    plt.show()
    # 绘制误差算法收敛曲线
    bmNet.TrendLine(plt)
    plt.show()

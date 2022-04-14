# -*- encoding:utf-8 -*-
import faiss
import numpy as np
import seaborn as sns
import matplotlib.pyplot as plt
import torch
import torch.nn.functional as F
from sklearn.datasets import make_blobs
from sklearn.metrics import euclidean_distances
from sklearn import datasets
def dataset_1():
    x1, y1 = datasets.make_circles(n_samples=1000, factor=0.35, noise=0.04)
    x2, y2 = datasets.make_blobs(n_samples=1000, centers=[[0.45, 0.5]], cluster_std=[[0.03]])
    x3, y3 = datasets.make_blobs(n_samples=1000, centers=[[-0.45, 0.5]], cluster_std=[[0.03]])

    X = np.concatenate((x1, x2, x3))
    # X = torch.tensor(X,dtype=torch.float32)
    # X = F.normalize(X,dim=1)
    Y = np.concatenate((y1, y2, y3))
    return X,Y
def dataset_2():
    X1, y1 = datasets.make_circles(n_samples=2000, factor=.6, noise=.05)
    X2, y2 = datasets.make_blobs(n_samples=500, n_features=2, centers=[[0, 0]], cluster_std=[[.1]],
                                 random_state=666)
    X = np.concatenate((X1, X2))
    # X = torch.tensor(X,dtype=np.float32)
    # X = F.normalize(X,dim=1)
    Y = np.concatenate((y1, y2))
    return X,Y
def dataset_3():
    x, y = datasets.make_circles(n_samples=5000, factor=.6, noise=.05)
    # x = torch.tensor(x)
    index = []
    for num in range(len(x)):
        if x[num, 0] < 0:
            index.append(num)
    x = np.delete(x, index, axis=0)
    index2 = []
    for num in range(len(x)):
        if x[num, 1] * x[num, 1] + x[num, 0] * x[num, 0] < 0.75 * 0.75:
            index2.append(num)
    x = np.delete(x, index2, axis=0)
    x2, y2 = datasets.make_blobs(n_samples=500, centers=[[0, 0]], cluster_std=[[0.2]])
    x = np.concatenate((x, x2))
    y = np.ones(len(x))
    return x,y
def dataset_4():
    X1,Y1= noisy_moons,_ = datasets.make_moons(n_samples=3000, noise=0.05)
    # X = torch.tensor(X1,dtype=np.float32)
    # X = F.normalize(X,dim=1)
    return X1,Y1
def dataset_5():
    X,Y = np.random.rand(3000, 2),np.ones(3000)
    # X = torch.tensor(X,dtype=np.float32)
    # X = F.normalize(X,dim=1)
    return X,Y
def dataset_6():
    x = []
    y = []
    with open("data/t4.txt", "r", encoding='utf-8') as f:
        data = f.read()
        data = data.split()

    for i in range(len(data)):
        if (i + 2) % 2 == 0:
            x.append(data[i])
        else:
            y.append(data[i])

    x = np.array(x, dtype=np.float32)
    x = F.normalize(torch.tensor(x),dim=0)
    y = np.array(y, dtype=np.float32)
    y = F.normalize(torch.tensor(y),dim=0)
    label = np.ones_like(x)
    X = np.vstack((x, y)).transpose()

    return X,label
class ISODATA():
    def __init__(self, designCenterNum, LeastSampleNum, StdThred, LeastCenterDist, iterationNum):
        #  指定预期的聚类数、每类的最小样本数、标准差阈值、最小中心距离、迭代次数
        self.K = designCenterNum
        self.thetaN = LeastSampleNum
        self.thetaS = StdThred
        self.thetaC = LeastCenterDist
        self.iteration = iterationNum

        # 初始化
        self.n_samples = 1500
        # 选一
        self.random_state1 = 200
        self.random_state2 = 160
        self.random_state3 = 170

        # self.data, self.label = make_blobs(n_samples=self.n_samples, random_state=self.random_state3)
        self.data, self.label = dataset_3()
        self.center = self.data[0, :].reshape((1, -1))
        self.centerNum = 1
        self.centerMeanDist = 0

        # seaborn风格
        sns.set()

    def updateLabel(self):
        """
            更新中心
        """
        for i in range(self.centerNum):
            # 计算样本到中心的距离
            distance = euclidean_distances(self.data, self.center.reshape((self.centerNum, -1)))
            # 为样本重新分配标签
            self.label = np.argmin(distance, 1)
            # 找出同一类样本
            index = np.argwhere(self.label == i).squeeze()
            sameClassSample = self.data[index, :]
            # 更新中心
            self.center[i, :] = np.mean(sameClassSample, 0)

        # 计算所有类到各自中心的平均距离之和
        for i in range(self.centerNum):
            # 找出同一类样本
            index = np.argwhere(self.label == i).squeeze()
            sameClassSample = self.data[index, :]
            # 计算样本到中心的距离
            distance = np.mean(euclidean_distances(sameClassSample, self.center[i, :].reshape((1, -1))))
            # 更新中心
            self.centerMeanDist += distance
        self.centerMeanDist /= self.centerNum

    def divide(self):
        # 临时保存更新后的中心集合,否则在删除和添加的过程中顺序会乱
        newCenterSet = self.center
        # 计算每个类的样本在每个维度的标准差
        for i in range(self.centerNum):
            # 找出同一类样本
            index = np.argwhere(self.label == i).squeeze()
            sameClassSample = self.data[index, :]
            # 计算样本到中心每个维度的标准差
            stdEachDim = np.mean((sameClassSample - self.center[i, :])**2, axis=0)
            # 找出其中维度的最大标准差
            maxIndex = np.argmax(stdEachDim)
            maxStd = stdEachDim[maxIndex]
            # 计算样本到本类中心的距离
            distance = np.mean(euclidean_distances(sameClassSample, self.center[i, :].reshape((1, -1))))
            # 如果最大标准差超过了阈值
            if maxStd > self.thetaS:
                # 还需要该类的样本数大于于阈值很多 且 太分散才进行分裂
                if self.centerNum <= self.K//2 or \
                        sameClassSample.shape[0] > 2 * (self.thetaN+1) and distance >= self.centerMeanDist:
                    newCenterFirst = self.center[i, :].copy()
                    newCenterSecond = self.center[i, :].copy()

                    newCenterFirst[maxIndex] += 0.5 * maxStd
                    newCenterSecond[maxIndex] -= 0.5 * maxStd

                    # 删除原始中心
                    newCenterSet = np.delete(newCenterSet, i, axis=0)
                    # 添加新中心
                    newCenterSet = np.vstack((newCenterSet, newCenterFirst))
                    newCenterSet = np.vstack((newCenterSet, newCenterSecond))

            else:
                continue
        # 更新中心集合
        self.center = newCenterSet
        self.centerNum = self.center.shape[0]

    def combine(self):
        # 临时保存更新后的中心集合,否则在删除和添加的过程中顺序会乱
        delIndexList = []

        # 计算中心之间的距离
        centerDist = euclidean_distances(self.center, self.center)
        centerDist += (np.eye(self.centerNum)) * 10**10
        # 把中心距离小于阈值的中心对找出来
        while True:
            # 如果最小的中心距离都大于阈值的话，则不再进行合并
            minDist = np.min(centerDist)
            if minDist >= self.thetaC:
                break
            # 否则合并
            index = np.argmin(centerDist)
            row = index // self.centerNum
            col = index % self.centerNum
            # 找出合并的两个类别
            index = np.argwhere(self.label == row).squeeze()
            classNumFirst = len(index)
            index = np.argwhere(self.label == col).squeeze()
            classNumSecond = len(index)
            newCenter = self.center[row, :] * (classNumFirst / (classNumFirst+ classNumSecond)) + \
                        self.center[col, :] * (classNumSecond / (classNumFirst+ classNumSecond))
            # 记录被合并的中心
            delIndexList.append(row)
            delIndexList.append(col)
            # 增加合并后的中心
            self.center = np.vstack((self.center, newCenter))
            self.centerNum -= 1
            # 标记，以防下次选中
            centerDist[row, :] = float("inf")
            centerDist[col, :] = float("inf")
            centerDist[:, col] = float("inf")
            centerDist[:, row] = float("inf")

        # 更新中心
        self.center = np.delete(self.center, delIndexList, axis=0)
        self.centerNum = self.center.shape[0]

    def drawResult(self):
        ax = plt.gca()
        ax.clear()
        ax.scatter(self.data[:, 0], self.data[:, 1], c=self.label, cmap="cool")
        # ax.set_aspect(1)
        # 坐标信息
        ax.set_xlabel('x axis')
        ax.set_ylabel('y axis')
        plt.show()


    def train(self):
        # 初始化中心和label
        self.updateLabel()
        self.drawResult()

        # 到设定的次数自动退出
        for i in range(self.iteration):
            # 如果是偶数次迭代或者中心的数量太多，那么进行合并
            if self.centerNum < self.K //2:
                self.divide()
            elif (i > 0 and i % 2 == 0) or self.centerNum > 2 * self.K:
                self.combine()
            else:
                self.divide()
            # 更新中心
            self.updateLabel()
            self.drawResult()
            print("中心数量：{}".format(self.centerNum))



if __name__ == "__main__":
    isoData = ISODATA(designCenterNum=4, LeastSampleNum=500, StdThred=0.001, LeastCenterDist=0.2, iterationNum=50)
    isoData.train()



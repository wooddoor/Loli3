#!/usr/bin/python 
# -*- coding: utf-8 -*-
#https://github.com/culat/compile_c51

import re
import sys


class Keil2Sdcc:
    def __init__(self):
        self.keySfr = re.compile(r'sfr\s*([^\s]*)\s*=\s*([^\s]*)\s*;')  # 获取sfr的全部名称及地址
        self.keySbit = re.compile(r'sbit\s*([^\s]*)\s*=\s*([^\s]*)\s*;')  # 获取sbit的全部名称及表达式
        self.keySbitName = re.compile(r'(.*)\^[0-9]')  # sbit名称正则
        # SDCC编译器标准
        self.newSfr = '__sfr __at {0} {1};'
        self.newSbit = '__sbit __at {0} {1};'

    def getSfr(self, fileText):
        resSfr = re.findall(self.keySfr, fileText)  # 获取sfr的全部名称及地址
        return resSfr

    def getSbit(self, fileText):
        resSbit = re.findall(self.keySbit, fileText)  # 获取sbit的全部名称及表达式

        # 获取sbit的全部名称
        sbitName = []
        for t in resSbit:
            sbitName.append(re.findall(self.keySbitName, t[1])[0])
        sbitName = list(set(sbitName))  # 去除重复名称

        # 构建sbit的名称与地址map
        sbitMap = {}
        for t in sbitName:
            address = re.findall(re.compile(r'sfr\s*' + t + '\s*=\s*([^\s]*)\s*;'), fileText)[0]
            sbitMap[t] = address

        # 替换所有sbit名称对应地址
        resSbitReplace = []  # 新的sbit数组
        for t in resSbit:
            rep = re.findall(self.keySbitName, t[1])[0]  # 获取地址对应的名字
            temp = t[1].replace(rep, sbitMap[rep])  # 将名字对应的表达式替换为地址
            temp = temp.replace('^', '+')  # 将^替换为+以便接下来计算
            temp = hex(eval(temp))  # 计算16进制地址值
            resSbitReplace.append([t[0], temp])  # 存为新数组

        return resSbitReplace

    def getText(self, fileText):
        resSfr = self.getSfr(fileText)
        resSbit = self.getSbit(fileText)

        newText = ''
        for s in resSfr:
            newText += self.newSfr.format(s[1], s[0]) + '\n'

        newText += '\n'

        for s in resSbit:
            newText += self.newSbit.format(s[1], s[0]) + '\n'

        return newText

    def getHText(self, fileText):
        # 添加H文件防止重复定义语句
        name = re.findall(re.compile(r'#ifndef\s*([^\s]*)\s*'), fileText)[0]
        newText = ''
        newText += '#ifndef ' + name + '\n'
        newText += '#define ' + name + '\n'
        newText += '\n' + self.getText(fileText)
        newText += '\n' + '#endif' + '\n'
        return newText


if __name__ == '__main__':

    fromFile = sys.argv[1]
    toFile = sys.argv[2]

    with open(fromFile, 'r', encoding='utf-8') as fr:
        text = fr.read()

    keil2Sdcc = Keil2Sdcc()
    text = keil2Sdcc.getHText(text)

    with open(toFile, 'w', encoding='utf-8') as fw:
        fw.write(text)

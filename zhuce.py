# -*- coding: utf-8 -*-
from time import sleep
import requests,re
from mayi import login,getMobile,getCode, blackNumber,pullNumber, sedMsg,submit
import os
import os.path
import string
#从文件获取ICCID号
usernmae = "qwerasdf1234"
passwd = "qwer1234"
itemID = 14422
f = open("ICCID", 'r')
#循环按行读取文件
for eachline in f:
	iccidObject = re.search("8961118261038498[0-9]{2}", eachline)
	ICCID = iccidObject.group(0)
	#print(ICCID)
#从蚂蚁获取手机号
	#绑定登录令牌
	token = login(usernmae, passwd)
	#释放所有手机号
	pullNumber(token)
	#获取手机号
	number = "null"
	while "null" is number:
		number = getMobile(token, itemID)
		sleep(5)
	print(number)
	#给手机发送验证码
	sedMsg(ICCID, number)
	#获取验证码
	Msg = "null"
	while "null" is Msg:
		sleep(5)
		Msg = getCode(token, itemID, number)
	print(Msg)
	#给服务器提交注册信息
	#通过URL完成注册
	string = submit(ICCID, number,Msg)
	
	#pullNumber(token)
f.close()





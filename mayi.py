# -*- coding: UTF-8 -*-
import urllib,urllib2
import requests,re
#username = "qwerasdf1234"
#用户名
#password = "qwer1234"
#密码
def login(username, password):
    url = "http://www.66yzm.com/api/admin/dengl?zhanghao={0}&mima={1}".format(username, password)
    try:
        res = requests.get(url).text
        dic= eval(res)
        token = dic["token"]
        return token
    except:
        return "null"
# 获取手机号
def getMobile(token,itemId):
    # 获取真实号段链接
    url = "http://www.66yzm.com/api/admin/getmobile?linpai={0}&itemid={1}".format(token, itemId)

    # 获取虚拟号段链接
    #url = "http://www.66yzm.com/api/admin/getmobile?linpai={0}&itemid={1}&xunihaoduan=1".format(token, itemId)
    try:
        res = requests.get(url).text
        number = re.findall("(1[0-9]{10})",res)[0]
        return number
    except:
        return "null"
# 获取验证码
def getCode(token,itemId,number):
    url ="http://www.66yzm.com/api/admin/shortmessage?linpai={0}&itemid={1}&mobile={2}" .format(token, itemId,number)
    try:
        res = requests.get(url).text
        # 正则匹配验证码，根据验证码位数修改花括号中的数字即可
        code = re.findall("([0-9]{6})", res)[0]
        return code
    except:
        return "null"
# 拉黑手机号
def blackNumber(token,itemId,number):
    url = "http://www.66yzm.com/api/admin/blacklist?linpai={0}&itemid={1}&mobile={2}".format(token, itemId,number)
    requests.get(url)
#释放手机号
def pullNumber(token):
    url = "http://www.66yzm.com/api/admin/releaseadd?linpai={0}".format(token)
    requests.get(url)
#提交验证码
def sedMsg(iccid,number):
    url = "http://wlk.hao2358.cn/Xm/sendSmsCode?iccid={0}&mobile={1}".format(iccid, number)
    string = requests.get(url)
    return string
#提交验证信息
def submit(iccid, number, msg):
    url = "http://wlk.hao2358.cn/Xm/BindCard?iccid={0}&mobile={1}&code={2}".format(iccid, number, msg)
    requests.get(url)

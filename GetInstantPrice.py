# Hello World program in Python
    
# -*- coding: UTF-8 -*-
import sys, urllib

stocks= ["600271",
"600067",
"000729",
"002798",
"000517",
"002645",
"600781",
"000928",
"002314",
"603123",
"600285",
"000528",
"600048",
"002415",
"000048",
"000585",
"002351",
"002613",
"002383",
"600459",
"600608",
"600051",
"002047",
"002042",
"600195",
"600261",
"000608",
"002155",
"600615",
"603021",
"600561",
"002173",
"600988",
"000995",
"600392",
"002601",
"002371",
"000976",
"601368",
"600083",
"002719",
"000839",
"600259",
"002392",
"000525",
"002193"]

fileHandle = open ( 'test.txt', 'w' ) 
for stock in stocks :
	if stock.startswith("6"):
		stock = "sh"+stock
	if stock.startswith("0"):
		stock = "sz"+stock
	url = "http://hq.sinajs.cn/list="+stock
	wp = urllib.urlopen(url)
	content = str(wp.read())
	fileHandle.write(stock[2:]+"\t"+content.split(",")[3]+"\n")
fileHandle.close()

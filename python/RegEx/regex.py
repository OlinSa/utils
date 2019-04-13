# coding:utf-8
import re

'''
match 从string开头进行匹配
'''
def match_case():
    pattern = re.compile(r'\d+') 
    result1 = re.match(pattern, '192abc')
    if result1:
        print(result1.group())
    else:
        print('case1 match failed')
    
    result2 = re.match(pattern, 'abc192')
    if result2:
        print(result2.group())
    else:
        print('case2 match failed')


'''
search 从string开始部位开始搜索
'''
def search_case():
    pattern = re.compile(r'\d+')
    result1 = re.search(pattern, 'abc192edf')
    if result1:
        print(result1.group())
    else:
        print('case1 match failed')

'''
将匹配的子串将string 分割后返回列表
'''
def splite_case():
    pattern = re.compile(r'\d+')
    print(re.split(pattern, 'A1B2C3D4'))

'''
搜索整个string,以列表返回能匹配的全部子串
'''
def findall_case():
    pattern = re.compile(r'\d+')
    print(re.findall(pattern, 'A1B2C3D4'))

'''
搜索整个string,以迭代器形式返回能匹配的全部Match对象
'''
def finditer_case():
    pattern = re.compile(r'\d+')
    matchiter = re.finditer(pattern, 'A1B2C3D4')
    for match in matchiter:
        print(match.group())

'''
替换每个匹配的子串,并返回替换后的字符串
'''
def sub_case():
    p = re.compile(r'(?P<word1>\w+) (?P<word2>\w+)')
    s = 'i say, hello world!'
    print(p.sub(r'\g<word2> \g<word1>', s))
    p = re.compile(r'(\w+) (\w+)')
    print(p.sub(r'\2 \1', s))
    def func(m):
        return m.group(1).title() + ' ' + m.group(2).title()
    print(p.sub(func, s))

'''
返回sub替换次数
'''
def subn_case():
    s = 'i say, hello world!'
    p = re.compile(r'(\w+) (\w+)')
    print(p.subn(r'\2 \1', s))
    def func(m):
        return m.group(1).title() + ' ' + m.group(2).title()
    print(p.subn(func, s))

if __name__=='__main__':
    match_case()
    search_case()
    splite_case()
    findall_case()
    finditer_case()
    sub_case()
    subn_case()
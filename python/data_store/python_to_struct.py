import requests
import urllib
from bs4 import BeautifulSoup
from lxml import etree
import json
import ssl

ssl._create_default_https_context = ssl._create_unverified_context


def ParseToJson(text):
    soup = BeautifulSoup(text, 'html.parser', from_encoding='utf-8')
    content = []
    for mulu in soup.find_all(class_='mulu'):
        h2 = mulu.find('h2')
        if h2 != None:
            h2_title = h2.string  # get title
            list = []
            # find all target which in URL and conetn
            for a in mulu.find(class_='box').find_all('a'):
                href = a.get('href')
                box_title = a.get('title')
                list.append({'href': href, 'box_title': box_title})
            content.append({'title': h2_title, 'content': list})

    with open('qiye.json', 'w') as fp:
        json.dump(content, fp=fp, indent=4)

def Schedule(blocknum, blocksize, totalsize):
    per = 100.0 * blocknum * blocksize / totalsize
    if per > 100:
        per = 100
    print('process: %d' % per)


def DownloadMedia(text):
    html = etree.HTML(text)
    img_urls = html.xpath('.//img/@src')
    i = 0
    for img_url in img_urls:
        complete_url = "https:"+img_url
        print(complete_url)
        urllib.request.urlretrieve(
            complete_url, 'img'+str(i) + '.jpg', Schedule)
        i += 1


if __name__ == "__main__":
    user_agent = 'Mozilla/4.0 (compatatible;MSIE 5.5; Windows NT)'
    headers = {'User-Agent': user_agent}
    r = requests.get('http://seputu.com/', headers=headers)
    # print(r.text)
    ParseToJson(r.text)

    r = requests.get(
        'https://www.ivsky.com/tupian/yanhua_v52229/', headers=headers)
    DownloadMedia(r.text)

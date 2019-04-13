from DataOutput import DataOutput
from HtmlDownloader import HtmlDownloader
from HtmlParser import HtmlParser
from URLManager import URLManager

import re
import ssl

ssl._create_default_https_context = ssl._create_unverified_context

class SpideMan(object):
    def __init__(self):
        self.manager = URLManager()
        self.downloader = HtmlDownloader()
        self.parser = HtmlParser()
        self.output = DataOutput()
    def crawl(self, root_url):
        self.manager.add_new_url(root_url)
        
        while self.manager.has_new_url() and self.manager.old_url_size() < 100:
            try:
                new_url = self.manager.get_new_url()
                html = self.downloader.download(new_url)
                new_urls, data = self.parser.parse(new_url, html)
                self.manager.add_new_urls(new_urls)
                self.output.store_data(data)
                print("catch %s link"%self.manager.old_url_size())
            except Exception as e:
                print(e)
        self.output.output_html()

if __name__ == "__main__":
    '''
    content = '/item/%E8%9C%98%E8%9B%9B/8135707'
    regex = re.compile('/item/\S+/\d+')
    print(regex.findall(content))
    '''
    spide_man = SpideMan()
    spide_man.crawl("https://baike.baidu.com/item/网络爬虫")


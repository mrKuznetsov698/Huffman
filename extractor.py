from pathlib import Path

from lxml import etree

# path = Path('~/Downloads/voina-i-mir.fb2')
# path = Path('/home/arsenii/second_home/BookShelf/Риордан Рик/Вселенная Перси Джексона и наследников богов/I Мир Перси Джексона/01. Перси Джексон и боги-олимпийцы/01. Перси Джексон и похититель молний (2009).fb2')
path = Path('/home/arsenii/Downloads/178303.fb2')

with open(path, encoding='utf8') as f:
    root = etree.parse(f).getroot()

f = open('percy_jackson_en.txt', 'w')

def handle_tag(el: etree._Element):
    if el.tag.endswith('p') and el.text is not None:
        f.write(el.text.replace(' ', ' ') + '\n')
    else:
        for sub in el:
            handle_tag(sub)


for child in root:
    if child.tag.endswith('body'):
        handle_tag(child)


f.close()
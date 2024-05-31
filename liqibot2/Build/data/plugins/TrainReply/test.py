import jieba_fast as jieba
import re
import os
import logging
from gensim.models.doc2vec import Doc2Vec
from gensim.models.doc2vec import TaggedDocument

logging.basicConfig(format='%(asctime)s : %(levelname)s : %(message)s', level=logging.INFO)

def load_text(count):
    sentences = []
    logfolder = r"C:\Users\liqi\Documents\GitHub\liqibot2\liqibot2\Build\data\logs/"
    files = os.listdir(logfolder)[:count]
    for file in files:
        with open(logfolder + file, encoding='utf-8', errors='ignore') as f:
            text = f.read()
            sentences.extend(re.findall('{"type":"Plain","text":"(.*?)"}',text))
    
    return sentences

def load_stoplist():
    stop_list = [line[:-1] for line in open(r"C:\Users\liqi\Downloads\中文停用词.txt", encoding='utf-8', errors='ignore')]
    return stop_list

def get_trainset(sentences, stop_list):
    x_train = []
    for i, each in enumerate(sentences):
        each_cut = jieba.cut(each)
        each_result = [word for word in each_cut if word not in stop_list]
        document = TaggedDocument(each_result, tags=[i])
        x_train.append(document)
    
    return x_train

def train_model(x_train):
    model = Doc2Vec(x_train, workers=6)
    model.train(x_train, total_examples=model.corpus_count, epochs=10)
    return model

def save_model(model):
    model.save('d2v.model')

def load_model():
    model = Doc2Vec.load('d2v.model')
    return model

def test_model(sentences, model, s):
    vector = model.infer_vector(jieba.cut(s), alpha=0.025, steps=500)
    sims = model.dv.most_similar([vector], topn=10)
    for count,sim in sims:
        sentence = sentences[count]
        print(sentence, sim)

def main():
    sentences = load_text(10)
    stop_list = load_stoplist()
    x_train = get_trainset(sentences, stop_list)
    model = train_model(x_train)
    save_model(model)


if __name__=='__main__':
    main()
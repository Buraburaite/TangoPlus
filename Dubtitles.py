# -*- coding: utf-8 -*-
class Dubtitles:
    def __init__(self, srtName):
        subs=open(srtName, "r", encoding="utf-8").read().replace("<b>","").replace("</b>","").replace("<i>","").replace("</i>","").split("\n")
        slides={}
        slide=""
        slideNum=-1
        for i in range(1, len(subs)):
            #print("Part: " + str(i) + ", " + subs[i])
            if "-->" in subs[i]:
                slideNum+=1
                slides.update({ slideNum : [[subs[i][0:12],subs[i][17:29]], ""] })
                try:
                    slides[slideNum - 1][1]=slide[:-(len(str(slideNum + 1)))]
                except:
                    pass
                slide=""
            else:
                slide+=subs[i]

        slides[slideNum][1]=slide

        self.slides=slides
        self.slideNum=slideNum

    def __iter__(self):
        return iter(self.slides)
    def __getitem__(self, index):
        return self.slides[index]
    def __repr__(self):
        return str(self.slides)


dubsubs=Dubtitles("static//example.srt")
print(dubsubs[1])
#print(dubsubs)
#for sub in dubsubs:
#    print(dubsubs[sub])

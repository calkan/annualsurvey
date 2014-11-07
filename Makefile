
PAPER = survey

TEX = $(wildcard ./*.tex) 

.PHONY: all, clean
all: $(TEX) $(FIGS) citemarker.c
	pdflatex $(PAPER) 
	pdflatex $(PAPER)
	gcc citemarker.c -o citemarker -O2
	gcc citemarker-tex.c -o citemarker-tex -O2

clean:
	rm -rf *.dvi *.log *.aux *.bbl *.blg *.pdf citemarker


file=main
all:
	latex2html -html_version 4.0,latin1,unicode -toc_depth 1 \
	 -split 3 -subdir -antialias_text -up_url ../index.html \
   -contents ../index.html \
   -style ../style.css ${file}.tex

clean:
	rm ${file}/*.html ${file}/*.png ${file}/*.aux ${file}/*.out ${file}/*.tex \
	${file}/*.jpg ${file}/*.pl *.aux *.log *.out ${file}/*.log ${file}/WARNINGS

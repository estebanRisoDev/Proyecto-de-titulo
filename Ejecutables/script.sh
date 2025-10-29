




for((i=1;i<=50;i++))
 do
	for((j=4;j<=6;j++))
	 do
  		./knn_with_query mat_65536_gauss_0$j $i QUERY4
		./knn_with_query mat_65536_random_0$j $i QUERY4
 	 done
 done




for((i=1;i<=50;i++))
	do
        	./knn_with_query final-evacuacion2 $i QUERY4
        	./knn_with_query final-hospitales2 $i QUERY4
        	./knn_with_query final-metros2 $i QUERY4
        	./knn_with_query final-museos2 $i QUERY4
        	./knn_with_query final-universidades2 $i QUERY4
        	./knn_with_query final-wifi2 $i QUERY4
        	./knn_with_query licencias $i QUERY4
        	./knn_with_query stream $i QUERY4
        	./knn_with_query street $i QUERY4
        	./knn_with_query crash $i QUERY4
        	./knn_with_query final-hospitales2 $i QUERY4
	done


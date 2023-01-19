all: test-clasificadores

download:
	if [ ! -f HT_Sensor_dataset.dat ] || [ ! -f HT_Sensor_metadata.dat ]; then \
		wget -N https://archive.ics.uci.edu/ml/machine-learning-databases/00362/HT_Sensor_UCIsubmission.zip; \
		unzip -n HT_Sensor_UCIsubmission.zip; \
		rm -r __MACOSX; \
		rm HT_Sensor_UCIsubmission.zip; \
		unzip -n HT_Sensor_dataset.zip; \
		rm HT_Sensor_dataset.zip; \
	fi
    
convertir-a-csv: download
	if [ ! -f HT_Sensor_dataset.csv ] || [ ! -f HT_Sensor_metadata.csv ]; then \
		python3 toCSV.py HT_Sensor_dataset.dat; \
		python3 toCSV.py HT_Sensor_metadata.dat; \
	fi

dataset-estimulo: convertir-a-csv
	if [ ! -f dataset_estimulos.csv ]; then \
		python3 dataset_estimulo.py; \
	fi

dataset-clasificar: dataset-estimulo
	if [ ! -f dataset_clasificador.csv ]; then \
		python3 dataset_clasificar.py; \
	fi

test-clasificadores: dataset-clasificar
	python3 test_clasificadores.py
	
clean:
	-rm -f *.csv
	-rm -f *.dat
	-rm -rf __pycache__

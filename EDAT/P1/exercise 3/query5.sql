SELECT z.NAME FROM (SELECT x.NAME, Count(*) AS most_rented_movie
		FROM (SELECT film_rental.NAME
			FROM   rental,
			(SELECT inventory_id, film_inventory.NAME
				FROM inventory,(SELECT film_id, film_name.NAME
						FROM film, (SELECT   language_id, NAME
								FROM language
								GROUP BY language_id) AS film_name
								WHERE film.language_id = film_name.language_id) AS film_inventory --SELECTS THE FILM ID AND THE NAME OF THE LANGAGE IT HAS BEEN RECORDED
						WHERE inventory.film_id = film_inventory.film_id) AS film_rental --SELECTS THE ID OF THE INVENTORY THAT HAS THAT FILM
				WHERE  film_rental.inventory_id=rental.inventory_id) AS x --IF THE MOVIE HAS BEEN RENTED AND IT IS IN THE INVENTORY IT TAKES THE NAME OF IT
		GROUP BY x.NAME
		ORDER BY most_rented_movie DESC) AS z --SELECTS ALL THE TIMES A FILM HAS BEEN RECORD IN A LANGUAGE
WHERE most_rented_movie IN (SELECT Max(most_rented_movie)
				FROM(SELECT x.NAME, Count(*) AS most_rented_movie
					FROM (SELECT film_rental.NAME
						FROM   rental,
						(SELECT inventory_id, film_inventory.NAME
							FROM inventory,(SELECT film_id, film_name.NAME
									FROM film, (SELECT   language_id, NAME
											FROM language
											GROUP BY language_id) AS film_name
											WHERE film.language_id = film_name.language_id) AS film_inventory--SELECTS THE FILM ID AND THE NAME OF THE LANGAGE IT HAS BEEN RECORDED
									WHERE inventory.film_id = film_inventory.film_id) AS film_rental --SELECTS THE ID OF THE INVENTORY THAT HAS THAT FILM
							WHERE  film_rental.inventory_id=rental.inventory_id) AS x --IF THE MOVIE HAS BEEN RENTED AND IT IS IN THE INVENTORY IT TAKES THE NAME OF IT
				GROUP BY x.NAME
				ORDER BY most_rented_movie DESC)AS y)--Y=SELECTS ALL THE TIMES A FILM HAS BEEN RECORD IN A LANGUAGE
									---PICKS THE TOP LANGUAGE A FILM HAS BEEN RECORDED
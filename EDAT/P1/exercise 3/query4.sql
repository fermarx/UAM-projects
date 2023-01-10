SELECT NAME
FROM language,
	(SELECT language_id, order_count
		FROM (SELECT language_id, Count(*) AS order_count
		FROM film
		GROUP BY language_id
		ORDER BY Count(*) DESC) AS aviable_languages --SELESCT ALL THE AVIABLE LANGUAGES
	WHERE order_count IN (SELECT Max(order_count)
				FROM (SELECT language_id, Count(*) AS order_count
					FROM film
					GROUP BY language_id
					ORDER BY Count(*) DESC) AS x)) AS id_movie -- SELECTS THE TOP LANGUAGE IN WICH MOVIES HAVE BEEN RECORD
WHERE language.language_id=id_movie.language_id --SELECTS THE NAME OF THE TOP LANGUAGE MOVIES HAVE BEEN RECORD
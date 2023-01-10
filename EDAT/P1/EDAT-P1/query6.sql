CREATE OR REPLACE VIEW customer_and_num_rented_movies AS
SELECT customer_id, max(count) AS num_film
FROM (SELECT customer_id, category_id, count(category_id)
	FROM (SELECT top_customer.customer_id, film_category.category_id
		FROM inventory, rental, film_category, (SELECT customer_id 
							FROM (SELECT customer_id, Count(*)
								FROM rental 
								GROUP BY customer_id 
								ORDER BY Count (*) DESC) AS customer 
							WHERE count IN (SELECT MAX(count) 
									FROM (SELECT customer_id, Count(*) 
										FROM rental 
										GROUP BY customer_id 
										ORDER BY Count (*) DESC) AS x))AS top_customer  --TOP CUSTOMERS
									WHERE rental.inventory_id = inventory.inventory_id
									AND rental.customer_id = top_customer.customer_id
									AND inventory.film_id = film_category.film_id) AS custumer_and_category -- PICKS THE CATEGORY ID OF ALL TH EMOVIES THE TOP CUSTOMERS RENTED
	GROUP BY category_id, customer_id
	ORDER BY category_id ASC) AS count_category --COUNTS ALL THE TIMES A DIFFERENT CATEGORY HAS BEEN RENTED FROM A DIFFERENT CUSTOMER
GROUP BY customer_id; -- IT GIVES YOU THE MOST RENTED CATEGORY OF THE TOP CUSTOMERS

CREATE OR REPLACE VIEW cost_and_category_id_count AS
SELECT customer_and_num_rented_movies.customer_id, count_category.category_id, count_category.count
FROM customer_and_num_rented_movies, (SELECT customer_id, category_id, count(category_id)
					FROM (SELECT top_customer.customer_id, film_category.category_id
						FROM inventory, rental, film_category, (SELECT customer_id 
											FROM (SELECT customer_id, Count(*)
												FROM rental 
												GROUP BY customer_id 
												ORDER BY Count (*) DESC) AS customer 
											WHERE count IN (SELECT MAX(count) 
													FROM (SELECT customer_id, Count(*) 
														FROM rental 
														GROUP BY customer_id 
														ORDER BY Count (*) DESC) AS x))AS top_customer --TOP CUSTOMERS
													WHERE rental.inventory_id = inventory.inventory_id
													AND rental.customer_id = top_customer.customer_id
													AND inventory.film_id = film_category.film_id) AS custumer_and_category-- PICKS THE CATEGORY ID OF ALL TH EMOVIES THE TOP CUSTOMERS RENTED
					GROUP BY category_id, customer_id
					ORDER BY category_id ASC) AS count_category--COUNTS ALL THE TIMES A DIFFERENT CATEGORY HAS BEEN RENTED FROM A DIFFERENT CUSTOMER
WHERE customer_and_num_rented_movies.num_film = count_category.count
AND customer_and_num_rented_movies.customer_id = count_category.customer_id;-- PICKS THE CUSTOMER ID AND THE CATEGORY ID AND SHOWS HOW MANY TIMES A CATEGORY HAS BEEN RENTED

SELECT * FROM cost_and_category_id_count
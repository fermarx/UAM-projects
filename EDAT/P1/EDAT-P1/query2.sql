SELECT first_name, last_name, customer.customer_id, count
FROM customer, (SELECT customer_id, count
		FROM (SELECT customer_id, Count(*)
			FROM rental
			GROUP BY customer_id
			ORDER BY Count (*) DESC) AS customer -- LIST OF HOW MANY MOVIES THE CUSTUMERS RENTED
		WHERE count IN (SELECT Max(count)
				FROM (SELECT customer_id, Count(*)
					FROM rental
					GROUP BY customer_id
					ORDER BY Count (*) DESC) AS x) ) AS id_client --X = CUSTOMER; ID_CLIENT SELECTS THE TOP ONE
WHERE id_client.customer_id = customer.customer_id -- CHOSE THE FIRST NAME, LAST NAME, ID AND COUNT
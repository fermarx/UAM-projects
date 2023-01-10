SELECT Count(*) AS number_rented_films,
       Extract (year FROM rental_date) AS year --PICKS THE YEAR OF THE DATE 
FROM   rental
GROUP  BY year
ORDER  BY ( year ) ASC --ORDERS THE YEAR ASC. AND COUNTS THE NUMBER OF FILMS RENTED EACH OF THEM 
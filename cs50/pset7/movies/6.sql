SELECT AVG(rating) FROM ratings
JOIN movies ON id = movie_id
WHERE year = 2012;
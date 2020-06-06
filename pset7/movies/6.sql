SELECT AVG(rating) FROM ratings
JOIN movies m on ratings.movie_id = m.id
WHERE year = 2012;
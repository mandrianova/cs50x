SELECT COUNT(title) FROM movies
JOIN ratings r on movies.id = r.movie_id
WHERE rating = 10.0 GROUP BY rating;
SELECT title FROM movies
WHERE id in
      (SELECT movie_id from stars
      WHERE person_id = (SELECT id FROM people WHERE name = "Johnny Depp")
        INTERSECT SELECT movie_id from stars
      WHERE person_id = (SELECT id FROM people WHERE name = "Helena Bonham Carter"));
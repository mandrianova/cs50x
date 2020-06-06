from flask import Flask, redirect, render_template, request, session
from flask_session import Session

app = Flask(__name__)
app.config["SESSION_PERMANENT"] = False  # the session is not permanent for this app
app.config["SESSION_TYPE"] = "filesystem"  # location for store data of the session
Session(app)  # turn on sessions for the app


@app.route("/")
def tasks():
    if "todos" not in session:
        session["todos"] = []
    return render_template("tasks.html", todos=session["todos"])


@app.route("/add", methods=["GET", "POST"])
def add():
    if request.method == "GET":
        return render_template("add.html")
    else:
        todo = request.form.get("task")
        session["todos"].append(todo)
        return redirect("/")





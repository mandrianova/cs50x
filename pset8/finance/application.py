import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash
from datetime import datetime

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    user_id = session.get("user_id")
    trades = db.execute("SELECT symbol, name, SUM(shares) as shares FROM trades WHERE user_id = :user_id GROUP BY symbol ORDER BY shares DESC", user_id=user_id)
    cash = db.execute("SELECT cash FROM users WHERE id = :user_id", user_id=user_id)[0]['cash']
    total = cash
    for trade in trades:
        if int(trade["shares"]) > 0:
            trade['price'] = lookup(trade["symbol"])['price']
            trade['total'] = round(int(trade['shares']) * float(trade['price']), 2)
            total += trade['total']
        if int(trade["shares"]) == 0:
            trades.remove(trade)
    return render_template("index.html", total=total, trades=trades, cash=cash)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        if not request.form.get("symbol") or not request.form.get("shares"):
            return apology("must provide symbol and shares", 403)
        if int(request.form.get("shares")) <= 0:
            return apology("Shares shoud be positive number", 403)
        result = lookup(request.form.get("symbol"))
        if not result:
            return apology("invalid symbol", 403)
        user_id = session.get("user_id")
        # Check cash
        cash = db.execute("SELECT cash FROM users WHERE id = :user_id", user_id=user_id)[0]['cash']
        cost = float(result['price']) * int(request.form.get('shares'))
        # Condition for cash
        if cash < cost:
            return apology(f"not enought cash: ${cash}. Cost of shares: ${cost}", 403)
        # minus cash
        db.execute("UPDATE users SET cash = :cash WHERE id = :id", cash=cash-cost, id=user_id)
        # insert buying in trades table
        db.execute("INSERT INTO trades (symbol, name, price, shares, date, type, user_id) VALUES (:symbol, :name, :price, "
           ":shares, :date, :type, :user_id)", symbol=result['symbol'], name=result['name'], price=float(result['price']),
           shares=int(request.form.get("shares")), date=datetime.now(), type='buy', user_id=user_id)
        return redirect('/')
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    user_id = session.get("user_id")
    trades = db.execute("SELECT symbol, price, shares, date FROM trades WHERE user_id = :user_id ORDER BY date DESC", user_id=user_id)
    return render_template("history.html", trades=trades)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
         # Ensure username was submitted
        if not request.form.get("symbol"):
            return apology("must provide symbol", 403)
        else:
            result = lookup(request.form.get("symbol"))
            if not result:
                return apology("invalid symbol", 403)
            return render_template('quote.html', result=result)
    else:
        return render_template('quote.html', result=None)


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
         # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)
        elif request.form.get("password") != request.form.get("password_repeat"):
            return apology("the passwords do not match", 403)
        # Ensure username is unique
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                              username=request.form.get("username"))
        if len(rows) > 0:
            return apology("username already registered", 403)
        # Query database for register user
        hash_password = generate_password_hash(request.form.get("password"))
        db.execute("INSERT INTO users (username, hash) VALUES (:username, :hash)",
                          username=request.form.get("username"), hash=hash_password)
        return redirect("/")
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    user_id = session.get("user_id")
    """Sell shares of stock"""
    if request.method == "GET":
        rows = db.execute("SELECT symbol, SUM(shares) AS count FROM trades WHERE user_id = :user_id GROUP BY symbol", user_id=user_id)
        symbols = []
        for row in rows:
            if row['count'] > 0:
                symbols.append(row['symbol'])
        return render_template("sell.html", symbols=symbols)
    else:
        if not request.form.get("symbol") or not request.form.get("shares"):
            return apology("must provide symbol and shares", 403)
        if int(request.form.get("shares")) <= 0:
            return apology("Shares shoud be positive number", 403)
        symbol = request.form.get("symbol")
        rows = db.execute("SELECT SUM(shares) AS count FROM trades WHERE user_id = :user_id AND symbol = :symbol "
                  "GROUP BY symbol", user_id=user_id, symbol=symbol)
        balance = int(rows[0]['count'])
        if int(request.form.get("shares")) > balance:
            return apology("not enough shares", 403)
        result = lookup(symbol)
        # Add trade
        db.execute("INSERT INTO trades (symbol, name, price, shares, date, type, user_id) VALUES (:symbol, :name, :price, "
           ":shares, :date, :type, :user_id)", symbol=symbol, name=result['name'], price=float(result['price']),
           shares=-int(request.form.get("shares")), date=datetime.now(), type='sell', user_id=user_id)
        # Add cash
        cost = float(result['price']) * int(request.form.get("shares"))
        db.execute("UPDATE users SET cash = cash + :cost WHERE id = :id", cost=cost, id=user_id)
        return redirect('/')


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)

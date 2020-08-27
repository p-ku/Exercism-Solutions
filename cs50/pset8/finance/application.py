import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

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

    # group by just to get the number of stocks so we can then loop by that
    rows = db.execute("SELECT symbol, SUM(shares) FROM transactions WHERE uid = :uid GROUP BY symbol",uid=session["user_id"])
    cashamount = db.execute("SELECT cash FROM users WHERE id = :uid",uid=session["user_id"])
    total = cashamount[0]["cash"]
    cashamount = usd(cashamount[0]["cash"])

    stock = []
    summed = []
    count = 0

    for row in rows:

        if row["SUM(shares)"] > 0:
            # print(count)
            stock.append(lookup(row["symbol"]))
            summed.append(stock[count]["price"] * rows[count]["SUM(shares)"])
            total = stock[count]["price"] * rows[count]["SUM(shares)"] + total
            stock[count]["price"] = usd(stock[count]["price"])
            summed[count] = usd(summed[count])
            count = count + 1
    total = usd(total)
    # print(len(stock))
    return render_template("index.html", **locals())

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        # Ensure symbol was submitted
        if not request.form.get("symbol"):
            return apology("must provide symbol", 403)

        # Ensure shares was submitted
        if not request.form.get("shares"):
            return apology("must provide shares", 403)

        stock = lookup(request.form.get("symbol"))

        # Ensure symbol exists
        if stock == None:
            return apology("no such symbol exists", 403)


        symbol = stock["symbol"]
        shares = request.form.get("shares")
        price = stock["price"]*float(shares)


        # Ensure funds available
        funds_available = db.execute("SELECT cash FROM users WHERE id = :uid",uid=session["user_id"])
        funds_available = float(funds_available[0]["cash"])

        if price > funds_available:
            return apology("not enough funds", 403)

        name = stock["name"].strip('\"')

        db.execute("INSERT INTO transactions (uid, symbol, shares, price, time) VALUES (:uid, :symbol, :shares, :price, (SELECT CURRENT_TIMESTAMP))",
        uid=session["user_id"], symbol=symbol, shares=shares, price=usd(price))
        funds_left = funds_available - price

        db.execute("UPDATE users SET cash = :money WHERE id = :uid",money=funds_left,uid=session["user_id"])

        rows = db.execute("SELECT symbol, SUM(shares) FROM transactions WHERE uid = :uid GROUP BY symbol",uid=session["user_id"])
        cashamount = funds_available
        total = cashamount
        summed = []
        stock = []
        count = 0
        for row in rows:
            if row["SUM(shares)"] > 0:
            # print(count)
                stock.append(lookup(row["symbol"]))
                summed.append(stock[count]["price"] * rows[count]["SUM(shares)"])
                total = stock[count]["price"] * rows[count]["SUM(shares)"] + total
                stock[count]["price"] = usd(stock[count]["price"])
                summed[count] = usd(summed[count])
                count = count + 1
        cashamount = usd(cashamount)
        total = usd(total)
        return render_template("index.html", **locals())

    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    # group by just to get the number of stocks so we can then loop by that
    rows = db.execute("SELECT symbol, shares, price, time FROM transactions WHERE uid = :uid",uid=session["user_id"])

    count = len(rows)
    # print(len(stock))
    return render_template("history.html", **locals())


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

        stock = lookup(request.form.get("symbol"))

        # Ensure symbol exists
        if stock == None:
            return apology("no such symbol exists", 403)

        name = stock["name"]
        price = usd(stock["price"])
        symbol = stock["symbol"]

        return render_template("quoted.html",**locals())

    else:
        return render_template("quote.html")

# My piece of "flair", funds can be added though a tab at the top
# Simply click "+Funds" and say how much cash to add

@app.route("/funds", methods=["GET", "POST"])
@login_required
def funds():
    if request.method == "POST":

        newfunds = float(request.form.get("funds"))

        # Ensure valid
        if newfunds <= 0:
            return apology("greater than zero pls", 403)



        funds_available = db.execute("SELECT cash FROM users WHERE id = :uid",uid=session["user_id"])
        funds_available = float(funds_available[0]["cash"])

        if funds_available < newfunds:
            return apology("more dough pls", 403)

        money = newfunds + funds_available
        total = money

        db.execute("UPDATE users SET cash = :money WHERE id = :uid",money=money,uid=session["user_id"])

        rows = db.execute("SELECT symbol, SUM(shares) FROM transactions WHERE uid = :uid GROUP BY symbol",uid=session["user_id"])

        summed = []
        stock = []
        count = 0

        for row in rows:

            if row["SUM(shares)"] > 0:
            # print(count)
                stock.append(lookup(row["symbol"]))
                summed.append(stock[count]["price"] * rows[count]["SUM(shares)"])
                total = stock[count]["price"] * rows[count]["SUM(shares)"] + total
                stock[count]["price"] = usd(stock[count]["price"])
                summed[count] = usd(summed[count])
                count = count + 1
        cashamount = usd(money)
        total = usd(total)

        return render_template("index.html",**locals())

    else:
        return render_template("funds.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure username unique
        elif request.form.get("username") in db.execute("SELECT username FROM users WHERE username = :username",
        username=request.form.get("username")):
            return apology("username taken", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Ensure password confirmation
        elif request.form.get("password") != request.form.get("confirmation"):
            return apology("password confirmation mismatch")

        # Insert into database
        username = request.form.get("username")
        password = request.form.get("password")
        username=request.form.get("username")
        hashed=generate_password_hash(request.form.get("password"), method='pbkdf2:sha256', salt_length=8)
        db.execute("INSERT INTO users (username, hash) VALUES (:username, :password)", username=username, password=hashed)

        # Remember which user has registered
#        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
        # group by just to get the number of stocks so we can then loop by that
    if request.method == "POST":

     # Ensure symbol was submitted
        if not request.form.get("symbol"):
            return apology("must provide symbol", 403)

        # Ensure shares was submitted
        if (not request.form.get("shares")) or (int(request.form.get("shares")) < 1):
            return apology("must provide positive shares", 403)



        stock = lookup(request.form.get("symbol"))
        symbol = stock["symbol"]
        shares = request.form.get("shares")
        price = stock["price"]*int(shares)
        name = stock["name"].strip('\"')

        funds_available = db.execute("SELECT cash FROM users WHERE id = :uid",uid=session["user_id"])
        funds_available = float(funds_available[0]["cash"])

        db.execute("INSERT INTO transactions (uid, symbol, shares, price, time) VALUES (:uid, :symbol, :shares, :price, (SELECT CURRENT_TIMESTAMP))",
        uid=session["user_id"], symbol=symbol, shares=-int(shares), price=usd(price))
        funds_left = funds_available + price

        db.execute("UPDATE users SET cash = :money WHERE id = :uid;",money=funds_left,uid=session["user_id"])

        rows = db.execute("SELECT symbol, SUM(shares) FROM transactions WHERE uid = :uid GROUP BY symbol",uid=session["user_id"])

        summed = []
        stock = []
        count = 0
        cashamount = funds_available
        total = cashamount

        for row in rows:
            if row["SUM(shares)"] > 0:
            # print(count)
                stock.append(lookup(row["symbol"]))
                summed.append(stock[count]["price"] * rows[count]["SUM(shares)"])
                total = stock[count]["price"] * rows[count]["SUM(shares)"] + total
                stock[count]["price"] = usd(stock[count]["price"])
                summed[count] = usd(summed[count])
                count = count + 1
        cashamount = usd(cashamount)
        total = usd(total)
        return render_template("index.html", **locals())

    else:
        rows = db.execute("SELECT symbol, SUM(shares) FROM transactions WHERE uid = :uid GROUP BY symbol",uid=session["user_id"])

        summed = []
        stock = []
        count = 0
        for row in rows:
            print(row)
            print(len(row))
            print(row["symbol"])
            if row["SUM(shares)"] > 0:
            # print(count)
                stock.append(lookup(row["symbol"]))
                summed.append(stock[count]["price"] * rows[count]["SUM(shares)"])
                # total = stock[count]["price"] * rows[count]["SUM(shares)"] + total
                stock[count]["price"] = usd(stock[count]["price"])
                summed[count] = usd(summed[count])

            count = count + 1
        print(len(stock))
        print(count)
        return render_template("sell.html", **locals())


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)

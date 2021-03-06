import os, datetime

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

    # Add positions to dictionary
    positions = db.execute("SELECT symbol, SUM(shares) AS shares FROM history WHERE id_user = :id GROUP BY symbol HAVING SUM(shares) > 0", id = session["user_id"])
    total = 0

    for i in range(len(positions)):

        info = lookup(positions[i]['symbol'])
        price = float(info['price'])

        positions[i]['price'] = usd(price)
        positions[i]['total'] = usd(positions[i]['shares'] * price)
        positions[i]['name'] = info['name']

        total += positions[i]['shares'] * price

    # Add cash position to dictionary
    cash = usd(db.execute("SELECT cash FROM users WHERE id = :id", id = session["user_id"])[0]['cash'])
    positions.append({"symbol": "CASH", "name": "", "shares": "", "price": "", "total": cash})

    total = usd(total + db.execute("SELECT cash FROM users WHERE id = :id", id = session["user_id"])[0]['cash'])

    return render_template("index.html", len = len(positions), positions = positions, total = total)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":

        symbol = request.form.get("symbol")
        shares = int(request.form.get("shares"))
        info = lookup(symbol)
        price = float(info["price"])

        # Give error if invalid stock symbol
        if info == None:
            return apology("invalid stock symbol", 403)

        # Give error if invalid number of shares
        if int(shares) < 1:
            return apology("must purchase positive amount of shares", 403)

        # Check cash balance
        cash = db.execute("SELECT cash FROM users WHERE id = :id", id = session["user_id"])[0]['cash']
        costs = price * shares
        if costs > cash:
            return apology("not enough cash for this transaction", 403)

        # Update trading history
        time = datetime.datetime.now()
        db.execute("INSERT INTO history (id_user, symbol, shares, price, time) VALUES (?, ?, ?, ?, ?)", (session["user_id"], symbol, shares, price, time))

        # Update cash balance
        db.execute("UPDATE users SET cash = ? WHERE id = ?", (cash - costs, session["user_id"]))
        #db.execute("UPDATE users SET cash = ? WHERE id = ?", (10000, session["user_id"]))

        # Show output
        return redirect("/")

    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():

    # Browse history
    positions = db.execute("SELECT symbol, shares, price, time FROM history WHERE id_user = :id", id = session["user_id"])

    # Adjust price formatting
    for i in range(len(positions)):
        positions[i]['price'] = usd(positions[i]['price'])

    # Return output to html
    return render_template("history.html", len = len(positions), positions = positions)


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

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        symbol = request.form.get("symbol")
        info = lookup(symbol)

        # Give error if invalid stock symbol
        if info == None:
            return apology("invalid stock symbol", 403)

        # Send quoted output to webpage
        quote = "A share of " + info['name'] + " (" + info['symbol'] + ") costs " + str(usd(info['price'])) + "."
        return render_template("quote.html", name = quote)

    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        elif request.form.get("password") != request.form.get("password_check"):
            return apology("passwords do not match", 403)

        # Ensure password is longer than three characters
        elif len(request.form.get("password")) < 4:
            return apology("password must be longer than three characters", 403)

        # Username already exists
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))
        if len(rows) == 1:
            return apology("username already exists", 403)

        # Insert new user into database
        username = request.form.get("username")
        password = generate_password_hash(request.form.get("password"))
        db.execute("INSERT INTO users (username, hash) VALUES(?, ?)", [username, password])

        # Redirect user to login page
        return redirect("/login")

    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    if request.method == "POST":

        symbol = request.form.get("symbol")
        shares = int(request.form.get("shares"))
        info = lookup(symbol)
        price = float(info["price"])

        # Give error if invalid stock symbol
        if info == None:
            return apology("invalid stock symbol", 403)

        # Check if positive number of shares
        if int(shares) < 1:
            return apology("must sell positive amount of shares", 403)

        # Check if number of shares in portfolio
        position = db.execute("SELECT SUM(shares) AS shares FROM history WHERE id_user = :id AND symbol = :symbol", id = session["user_id"], symbol = symbol)[0]["shares"]
        if shares > position:
            return apology("you do not own a sufficient number of shares")

        # Update trading history
        time = datetime.datetime.now()
        db.execute("INSERT INTO history (id_user, symbol, shares, price, time) VALUES (?, ?, ?, ?, ?)", (session["user_id"], symbol, -shares, price, time))

        # Update cash balance
        cash = db.execute("SELECT cash FROM users WHERE id = :id", id = session["user_id"])[0]['cash']
        db.execute("UPDATE users SET cash = ? WHERE id = ?", (cash + shares * price, session["user_id"]))

        return redirect("/")

    else:
        return render_template("sell.html")


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)

import os

# Import necessary modules
from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash
import datetime

# Import custom helper functions
from helpers import apology, login_required, lookup, usd

# Configure the Flask application
app = Flask(__name__)

# Set up custom Jinja filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


# Set up after_request function to ensure responses aren't cached
@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Home page to show portfolio of stocks
@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    # Fetch user's transactions and cash balance
    userTrans = db.execute(
        "SELECT symbol, SUM(shares) AS shares, price FROM transactions WHERE user_id = ? GROUP BY symbol",
        session["user_id"],
    )
    dbCash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
    cash = dbCash[0]["cash"]

    return render_template("index.html", data=userTrans, cash=cash)


# Buy route to buy shares of stock
@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        symbol = request.form.get("symbol")
        if not symbol:
            return apology("Please provide a symbol")
        shares = request.form.get("shares")
        if not shares or not shares.isdigit():
            return apology("Please provide a valid shares amount")
        actualShares = int(shares)
        if actualShares <= 0:
            return apology("Please provide a positive shares amount")
        stock = lookup(symbol)
        if stock is None:
            return apology("Please provide a correct symbol")
        # Calculate transaction value and check if user has enough cash
        value = round(stock["price"] * actualShares, 2)
        dbCash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
        cash = dbCash[0]["cash"]
        if value > cash:
            return apology("Not enough money")
        newAmount = cash - value
        db.execute(
            "UPDATE users SET cash = ? WHERE id = ?", newAmount, session["user_id"]
        )
        date = datetime.datetime.now()
        # Add a new transaction record
        db.execute(
            "INSERT INTO transactions (user_id, shares, symbol, date, price) VALUES (?, ?, ?, ?, ?)",
            session["user_id"],
            actualShares,
            stock["symbol"],
            date,
            stock["price"],
        )
        flash("Bought!")
        return redirect("/")
    else:
        return render_template("buy.html")


# Route to show history of transactions
@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    transactions = db.execute(
        "SELECT * FROM transactions WHERE user_id = ?", session["user_id"]
    )
    return render_template("history.html", transactions=transactions)


# Route to change user password
@app.route("/change_password", methods=["GET", "POST"])
@login_required
def change_password():
    """Change user password"""
    if request.method == "GET":
        return render_template("change_password.html")
    else:
        oldPassword = request.form.get("old_password")
        newPassword = request.form.get("new_password")
        confirmation = request.form.get("confirmation")
        user = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])
        oldHash = user[0]["hash"]

        if not (newPassword == confirmation):
            return apology("New password and confirmation do not match")

        if oldPassword == "" or newPassword == "" or confirmation == "":
            return apology("Fill out all fields, please")

        if not check_password_hash(oldHash, oldPassword):
            return apology("Incorrect old password")

        newHash = generate_password_hash(newPassword)
        db.execute(
            "UPDATE users SET hash = ? WHERE id = ?", newHash, session["user_id"]
        )
        flash("Password changed!")
        return redirect("/")


# Route to log user in
@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""
    # Forget any user_id
    session.clear()

    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]
        # Redirect user to home page
        return redirect("/")
    else:
        return render_template("login.html")


# Route to log user out
@app.route("/logout")
def logout():
    """Log user out"""
    # Forget any user_id
    session.clear()
    # Redirect user to login form
    return redirect("/")


# Route to get stock quote
@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        symbol = request.form.get("symbol")
        if not symbol:
            return apology("Missing symbol")

        quote_info = lookup(symbol)
        if quote_info is None:
            return apology("Invalid symbol")
        else:
            quote_info["price"] = float(quote_info["price"])
            return render_template(
                "quoted.html",
                name=quote_info["name"],
                symbol=quote_info["symbol"],
                price=quote_info["price"],
            )
    else:
        return render_template("quote.html")


# Route to register user
@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        name = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")
        hash = generate_password_hash(password)
        existing_user = db.execute("SELECT * FROM users WHERE username = ?", name)

        if existing_user:
            return apology("Username is already taken")

        if not (password == confirmation):
            return apology("Password and confirmation not the same")

        if name == "" or password == "" or confirmation == "":
            return apology("Fill out all fields please")

        db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", name, hash)
        return redirect("/")
    else:
        return render_template("registration.html")


# Route to sell shares of stock
@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "GET":
        # Fetch symbols of stocks the user owns
        symbols = db.execute(
            "SELECT symbol FROM transactions WHERE user_id = ? GROUP BY symbol HAVING SUM(shares) > 0",
            session["user_id"],
        )
        symbols = [row["symbol"] for row in symbols]
        return render_template("sell.html", symbols=symbols)
    else:
        symbol = request.form.get("symbol")
        if not symbol:
            return apology("Please provide a symbol")
        shares = request.form.get("shares")
        if not shares:
            return apology("Please provide a shares amount")
        shares = int(shares)
        if shares <= 0:
            return apology("Please provide a positive shares amount")
        stock = lookup(symbol)
        if stock is None:
            return apology("Please provide a correct symbol")
        userShares = db.execute(
            "SELECT SUM(shares) AS total_shares FROM transactions WHERE user_id = ? AND symbol = ?",
            session["user_id"],
            symbol,
        )
        numUserShares = userShares[0]["total_shares"]
        if shares > numUserShares:
            return apology("Selling more shares than you have")
        value = stock["price"] * shares
        dbCash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
        cash = dbCash[0]["cash"]
        newAmount = cash + value
        db.execute(
            "UPDATE users SET cash = ? WHERE id = ?", newAmount, session["user_id"]
        )
        date = datetime.datetime.now()
        # Insert new transaction record for the sale
        db.execute(
            "INSERT INTO transactions (user_id, shares, symbol, date, price) VALUES (?, ?, ?, ?, ?)",
            session["user_id"],
            (-1) * shares,
            stock["symbol"],
            date,
            stock["price"],
        )
        flash("Sold!")
        return redirect("/")

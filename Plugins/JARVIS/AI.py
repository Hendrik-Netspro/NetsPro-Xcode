import re
import sys
import time
import threading

from ollama import generate
import Tools as t


# ===================== THINKING INDICATOR =====================
class ThinkingIndicator:
    def __init__(self, text="==> Jarvis denkt"):
        self.text = text
        self._stop = threading.Event()
        self._thread = None

    def _run(self):
        frames = ["⠋", "⠙", "⠹", "⠸", "⠼", "⠴", "⠦", "⠧", "⠇", "⠏"]
        i = 0
        while not self._stop.is_set():
            frame = frames[i % len(frames)]
            sys.stderr.write(f"\r{self.text} {frame}")
            sys.stderr.flush()
            time.sleep(0.08)
            i += 1

    def __enter__(self):
        self._stop.clear()
        self._thread = threading.Thread(target=self._run, daemon=True)
        self._thread.start()
        return self

    def __exit__(self, exc_type, exc, tb):
        self._stop.set()
        if self._thread is not None:
            self._thread.join(timeout=0.3)
        sys.stderr.write("\r" + " " * (len(self.text) + 4) + "\r")
        sys.stderr.flush()


# ===================== RULE ROUTER =====================
def first_routing(question):
    q = question.lower().strip()

    # SMART Prefix
    if q.startswith(("sei genau", "genau", "detailliert")):
        return "S"

    # FAST Prefix
    if q.startswith(("schnell", "fast", "quick")):
        return "F"

    q_clean = re.sub(r"[^\w\s]", " ", q)

    # FAST Keywords
    if any(word in q_clean for word in [
        "schnell", "sofort", "jetzt", "beeil", "mach hin",
        "zack", "kurz", "quick", "fast"
    ]):
        return "F"

    # Smalltalk
    if any(word in q_clean for word in [
        "hi", "hello", "hey", "hallo", "moin", "servus", "yo",
        "wie geht", "alles klar", "was geht", "na"
    ]):
        return "F"

    # Witze
    if any(word in q_clean for word in ["witz", "lustig"]):
        return "F"

    # Tools
    if any(word in q_clean for word in ["zeit", "uhr", "datum"]):
        return "T"

    if re.search(r"\d+\s*[+\-*/:]\s*\d+", q):
        return "T"

    # SMART
    if any(word in q_clean for word in [
        "erkläre", "warum", "wieso", "wie funktioniert",
        "code", "python", "c++", "klasse", "vererbung"
    ]):
        return "S"

    if len(q) > 120:
        return "S"

    return None


# ===================== FAST ROUTER =====================
def ai_router_fast(question):
    prompt = f"""Antworte nur mit genau einem Buchstaben: F, S, T oder N.

Regeln:
- T nur bei Zeit oder einfacher Rechnung.
- Zeit: uhr, uhrzeit, datum, wie spät.
- Rechnung: +, -, *, /, :
- S nur wenn die Frage klar eine ausführliche Erklärung braucht.
- Sonst F.
- Unsicher -> F.
- Kein Zusatztext.

Frage: {question}
Antwort:
"""

    with ThinkingIndicator():
        response = generate(model="qwen2:1.5b", prompt=prompt)

    answer = response["response"].strip().upper()
    if answer in ["F", "S", "T", "N"]:
        return answer
    return "N"


# ===================== SMART ROUTER =====================
def ai_router(question):
    prompt = f"""Antworte nur mit genau einem Buchstaben: F, S, T oder N.

Bedeutung:
F = einfache normale Antwort
S = ausführlich erklären
T = Tool nötig
N = unklar

Regeln:
- T bei Zeit oder Rechnung.
- S nur bei klar komplexen Erklärfragen.
- Sonst F.
- Kein Zusatztext.

Frage: {question}
Antwort:
"""

    with ThinkingIndicator():
        response = generate(model="phi3.5:latest", prompt=prompt)

    answer = response["response"].strip().upper()
    if answer in ["F", "S", "T"]:
        return answer
    return "F"


# ===================== CLEANER =====================
def clean_output(text):
    banned_parts = ["User:", "Assistant:", "In simple terms", "奔"]
    for bad in banned_parts:
        if bad in text:
            text = text.split(bad)[0]
    return text.strip()


def stream_model_response(model, prompt):
    chunks = []
    for part in generate(model=model, prompt=prompt, stream=True):
        token = part.get("response", "")
        if not token:
            continue
        chunks.append(token)
        print(token, end="", flush=True)
    print()
    return clean_output("".join(chunks))


# ===================== FAST AI =====================
def fast_ai(question, stream=False):
    prompt = f"""Du bist Jarvis.
Antworte nur auf Deutsch.
Kurz, klar, natürlich.
Maximal 6 Sätze.
Keine Rollen.
Keine Dialoge.
Keine Meta-Kommentare.
Keine erfundenen Fakten.
Wenn etwas unklar ist, antworte trotzdem sinnvoll und direkt.
Nutze Sir um den nutzer anzusprechen.

Frage: {question}
Antwort:
"""

    if stream:
        return stream_model_response("qwen2:1.5b", prompt)

    with ThinkingIndicator():
        response = generate(model="qwen2:1.5b", prompt=prompt)

    return clean_output(response["response"].strip())


# ===================== SMART AI =====================
def smart_ai(question, stream=False):
    prompt = f"""Du bist Jarvis.
Antworte nur auf Deutsch.
Erkläre klar, ordentlich und verständlich.
Nutze kurze Absätze oder wenige Punkte, wenn es hilft.
Bleibe direkt bei der Frage.
Keine Rollen.
Keine Dialoge.
Keine Meta-Kommentare.
Keine erfundenen Fakten.

Frage: {question}
Antwort:
"""

    if stream:
        return stream_model_response("llama3:latest", prompt)

    with ThinkingIndicator():
        response = generate(model="llama3:latest", prompt=prompt)

    return clean_output(response["response"].strip())


# ===================== TOOL AI =====================
def tool_ai(question, debug=False):
    prompt = f"""Du bist Jarvis.
Entscheide, ob genau ein Tool benutzt werden soll.

Erlaubte Tools:
- time
- calcplus
- calcminus
- calcmulti
- calcdivide

Nutze ein Tool nur bei:
- Uhrzeit, Zeit, Datum -> time
- a+b -> calcplus
- a-b -> calcminus
- a*b -> calcmulti
- a/b or a:b -> calcdivide

Wichtig:
- Bei Rechnung immer nur die zwei Zahlen aus der Frage nehmen.
- Bei time immer genau so schreiben:
TOOL:time
ARGS:none
- Bei Rechnen immer genau so schreiben:
TOOL:<toolname>
ARGS1:<erste zahl>
ARGS2:<zweite zahl>
- Kein Zusatztext vor oder nach TOOL/ARGS.
- Wenn kein Tool passt, antworte normal auf Deutsch.

Beispiele:
Frage: Wie spät ist es?
TOOL:time
ARGS:none

Frage: Was ist 15:8?
TOOL:calcdivide
ARGS1:15
ARGS2:8

Frage: Was ist 15/8?
TOOL:calcdivide
ARGS1:15
ARGS2:8

Frage: Was ist 15*8?
TOOL:calcmulti
ARGS1:15
ARGS2:8

Frage: {question}
Antwort:
"""

    with ThinkingIndicator():
        response = generate(model="qwen2:1.5b", prompt=prompt)

    text = response["response"].strip()
    lines = text.splitlines()

    tool = None
    arg1 = None
    arg2 = None

    for line in lines:
        line = line.strip()

        if line.startswith("TOOL:"):
            tool = line.split("TOOL:", 1)[1].strip().lower()

        elif line.startswith("ARGS1:"):
            value = line.split("ARGS1:", 1)[1].strip()
            if value.isdigit() or (value.startswith("-") and value[1:].isdigit()):
                arg1 = int(value)

        elif line.startswith("ARGS2:"):
            value = line.split("ARGS2:", 1)[1].strip()
            if value.isdigit() or (value.startswith("-") and value[1:].isdigit()):
                arg2 = int(value)

        elif line.startswith("ARGS:"):
            value = line.split("ARGS:", 1)[1].strip()
            if "," in value:
                left, right = value.split(",", 1)
                left = left.strip()
                right = right.strip()
                if left.isdigit() or (left.startswith("-") and left[1:].isdigit()):
                    arg1 = int(left)
                if right.isdigit() or (right.startswith("-") and right[1:].isdigit()):
                    arg2 = int(right)

    if tool == "time":
        tool_response = t.clock()
        answer = f"Die Uhrzeit ist {tool_response}."
    elif tool in ["calcplus", "calcminus", "calcmulti", "calcdivide"]:
        if arg1 is None or arg2 is None:
            if debug:
                return {
                    "answer": "Tool-Fehler: ARGS1 oder ARGS2 fehlt.",
                    "tool": tool,
                    "args": {"ARGS1": arg1, "ARGS2": arg2},
                    "tool_result": None,
                    "raw_tool_output": text,
                }
            return "Tool-Fehler: ARGS1 oder ARGS2 fehlt."

        if tool == "calcplus":
            tool_response = t.calcplus(arg1, arg2)
        elif tool == "calcminus":
            tool_response = t.calcminus(arg1, arg2)
        elif tool == "calcmulti":
            tool_response = t.calcmulti(arg1, arg2)
        else:
            tool_response = t.calcdivide(arg1, arg2)

        answer = f"Ergebnis: {tool_response}."
    else:
        if debug:
            return {
                "answer": text,
                "tool": tool,
                "args": {"ARGS1": arg1, "ARGS2": arg2},
                "tool_result": None,
                "raw_tool_output": text,
            }
        return text

    answer = answer.strip()

    if debug:
        return {
            "answer": answer,
            "tool": tool,
            "args": {"ARGS1": arg1, "ARGS2": arg2},
            "tool_result": tool_response,
            "raw_tool_output": text,
        }

    return answer


# ===================== CORE LOGIC =====================
def ask_jarvis(question, stream=False):
    route = first_routing(question)

    if route is None or route == "N":
        route = ai_router_fast(question)
        if route == "N":
            route = ai_router(question)

    tool = None
    args = None
    tool_result = None
    raw_tool_output = None

    if route == "F":
        answer = fast_ai(question, stream=stream)
    elif route == "S":
        answer = smart_ai(question, stream=stream)
    elif route == "T":
        tool_debug = tool_ai(question, debug=True)
        answer = tool_debug["answer"]
        tool = tool_debug["tool"]
        args = tool_debug["args"]
        tool_result = tool_debug["tool_result"]
        raw_tool_output = tool_debug["raw_tool_output"]
    else:
        answer = "Ich bin gerade etwas verwirrt. Frag nochmal kurz und direkt."

    return {
        "input": question,
        "route": route,
        "answer": answer,
        "tool": tool,
        "args": args,
        "tool_result": tool_result,
        "raw_tool_output": raw_tool_output,
    }


# ===================== CLI OUTPUT =====================
def cli_run(question, debug=False, stream=False):
    result = ask_jarvis(question, stream=stream)

    if debug:
        print("========== DEBUG ==========")
        print("INPUT       :", result["input"])
        print("ROUTE       :", result["route"])

        if result["route"] == "T":
            print("TOOL        :", result.get("tool"))
            print("ARGS        :", result.get("args"))
            print("TOOL RESULT :", result.get("tool_result"))
            print("RAW TOOL    :", result.get("raw_tool_output"))

        print("---------- OUTPUT ---------")
        print(result["answer"])
        print("===========================")
    else:
        if stream and result["route"] in ["F", "S"]:
            return
        print(result["answer"])


# ===================== MAIN =====================
def main():
    if len(sys.argv) < 2:
        print("Keine Frage übergeben.")
        return

    debug_mode = False
    stream_mode = False
    args = sys.argv[1:]

    while args and args[0] in ["--debug", "--stream"]:
        if args[0] == "--debug":
            debug_mode = True
        elif args[0] == "--stream":
            stream_mode = True
        args = args[1:]

    if not args:
        print("Keine Frage übergeben.")
        return

    user_question = " ".join(args)
    cli_run(user_question, debug=debug_mode, stream=stream_mode)


if __name__ == "__main__":
    main()

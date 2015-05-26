
saveOptions = ->
  options = {}
  for el in $('textarea, select, [type="hidden"], [type="password"], [type="text"], [type="radio"]:checked, [type="checkbox"]:checked')
    $el = $(el)
    options[$el.attr('name')] = $el.val()
  options

$ ->
  $("#b-cancel").on 'click', (e) ->
    document.location = "pebblejs://close"

  $("#b-submit").on 'click', (e) ->
    document.location = "pebblejs://close#" + encodeURIComponent(JSON.stringify(saveOptions()))

  # Set form values to whatever is passed in.
  hash = window.location.hash.substring(1)
  if hash
    obj = jQuery.parseJSON(decodeURIComponent(hash))
    for key, val of obj
      $el = $("[name=#{key}]")
      $el.val(val)
